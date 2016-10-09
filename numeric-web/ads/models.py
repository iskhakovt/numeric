# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


from django.db import models
from django.utils.crypto import get_random_string
from jsonfield import JSONField


QUERY_STATUS = (
    ('PE', 'PENDING'),
    ('OK', 'DONE'),
    ('FL', 'FAILED'),
)


class Query(models.Model):
    id = models.CharField(max_length=32, primary_key=True)
    status = models.CharField(max_length=2, choices=QUERY_STATUS)
    time = models.FloatField()
    result = JSONField()

    def get_dict(self):
        return {
            'status': self.status,
            'time:': self.time,
            'result': self.result.serialize()
        }

    @classmethod
    def create(cls):
        return cls.objects.create(
            id = get_random_string(length=32),
            status='PE'
        )
