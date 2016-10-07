# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


import os

from django.core.wsgi import get_wsgi_application

os.environ.setdefault("DJANGO_SETTINGS_MODULE", "numeric_web.settings")

application = get_wsgi_application()
