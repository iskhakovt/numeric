# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


from django.http import JsonResponse, HttpResponseBadRequest
from django.shortcuts import get_object_or_404, render

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
    for arg in problem.args:
        if arg not in request.POST:
            return HttpResponseBadRequest()
        args[arg] = request.POST[arg]

    query = Query.create()
    query.save()

    docker = Docker(query, problem.func, args)
    docker.run.delay()


def api_result(request, id):
    query = get_object_or_404(Query, id=id)

    if query.status != 'OK':
        return JsonResponse({'status': query.status})

    return JsonResponse(query.get_dict())
