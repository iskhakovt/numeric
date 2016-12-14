# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


from django.contrib import admin

from .docker import Docker
from .models import Query
from .problems import PROBLEMS


class QueryAdmin(admin.ModelAdmin):
    list_display = ['problem', 'status', 'time']

    def restart(self, request, queryset):
        for query in queryset:
            query.status = 'PE'
            query.save()

            problem = PROBLEMS[query.problem]
            docker = Docker(query, problem.func, query.args)
            docker.run.delay()

    restart.short_description = 'Restart queries'
    actions = [restart]


admin.site.register(Query, QueryAdmin)
