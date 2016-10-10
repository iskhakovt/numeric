
## Setting up

1. Set up virtualenv
  ```bash
python3 -m venv venv
. venv/bin/activate
pip install --upgrade pip
pip install -r requirements.txt
```

2. Install JS packages
  ```bash
npm install
```

3. Set up database
  ```bash
./manage.py makemigrations
./manage.py migrate
```

4. Create an admin
  ```bash
./manage.py createsuperuser
```


## Running (in develompent mode)

1. Redis
  ```bash
redis-server
```

2. Celery
  ```bash
celery -A numeric_web worker --loglevel=info
```

3. Webpack
  ```bash
./node_modules/.bin/webpack --config webpack.config.js --watch
```

4. Django
  ```bash
./manage.py runserver
```
