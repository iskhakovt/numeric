# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


from django.db import models
from django.utils.crypto import get_random_string
from jsonfield import JSONField
import json


QUERY_STATUS = (
    ('PE', 'PENDING'),
    ('OK', 'DONE'),
    ('FL', 'FAILED'),
)


class Query(models.Model):
    id = models.CharField(max_length=32, primary_key=True)
    status = models.CharField(max_length=2, choices=QUERY_STATUS, default='PE')
    time = models.FloatField(default=0.0)
    problem = models.CharField(max_length=100)
    args = JSONField(default={})
    result = JSONField(default={})

    def get_dict(self):
        return {
            'status': self.status,
            'time:': self.time,
            'args': self.args.serialize(),
            'result': self.result.serialize()
        }

    @classmethod
    def create(cls, problem, args):
        return cls.objects.create(
            id=get_random_string(length=32),
            problem=problem,
            args=json.dumps(args)
        )
