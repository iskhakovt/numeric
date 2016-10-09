// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License


import React from 'react'
import { render } from 'react-dom'
import { Router, Route, Link, browserHistory } from 'react-router'

import Problems from './problems'
import Problem from './problem'
import Result from './result'


render(
  <Router history={browserHistory}>
    <Route path='/' component={Problems}>
      <Route path='problem'>
        <Route path=':problem' component={Problem}/>
      </Route>
      <Route path='result' component={Result}>
        <Route path=':problem' component={Result}/>
      </Route>
    </Route>
  </Router>,
  document.getElementById('root')
);

// <Route path="*" component={NoMatch}/>
