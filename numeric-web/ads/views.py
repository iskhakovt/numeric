# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


from django.http import JsonResponse, HttpResponseBadRequest
from django.shortcuts import get_object_or_404, render
import json

from .docker import Docker
from .models import Query
from .problems import PROBLEMS


def index(request):
    return render(request, 'index.html')


def api_problems(request):
    return JsonResponse({
        problemName: {
            'short_description': PROBLEMS[problemName].short_description
        } for problemName in PROBLEMS
    })


def api_problem(request, problemName):
    if problemName not in PROBLEMS:
        return HttpResponseBadRequest()
    return JsonResponse(PROBLEMS[problemName].get_dict())


def api_query(request, problemName):
    if problemName not in PROBLEMS:
        return HttpResponseBadRequest()
    problem = PROBLEMS[problemName]
    args = {}

    try:
        for arg in problem.args:
            if arg not in request.POST:
                return HttpResponseBadRequest()

            value = json.loads(request.POST[arg])
            if problem.args[arg].isFunction:
                args[arg] = process_csv(value)
            else:
                args[arg] = float(value)
    except (TypeError, IndexError, KeyError, ValueError, AssertionError):
        return HttpResponseBadRequest()


    query = Query.create(problemName, args)
    query.save()

    docker = Docker(query, problem.func, args)
    docker.run.delay()

    return JsonResponse({'query': query.id})


def api_result(request, id):
    query = get_object_or_404(Query, id=id)
    return JsonResponse(query.get_dict())


def process_csv(data):
    x, y = [], []

    for elem in data:
        assert len(elem) == 2
        x.append(float(elem[0]))
        y.append(float(elem[1]))

    return x, y
