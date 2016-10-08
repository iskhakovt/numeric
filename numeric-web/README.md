
## Setting up

1. Install Python packages
  ```bash
pip3 install -r requirements.txt # pip if you use Python 3 by default
```

2. Install JS packages
  ```bash
npm install
```

3. Set up a DB
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
python3 manage.py runserver
```
