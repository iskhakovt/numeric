// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License


import React from 'react'

import $ from 'jquery-ajax'
import _ from 'lodash'
import TeX from 'react-components/js/tex.jsx'

// import { Link } from 'react-router'
// import { Nav, Navbar, NavItem } from 'react-bootstrap'
// import { LinkContainer } from 'react-router-bootstrap'


class Problem extends React.Component {
  constructor(props, context) {
    super(props, context);
    this.state = {
      problemName: null,
      problem: null
    };
  }

  componentDidMount() {
    this.request = null;
    this.load(this.props.params.problem);
  }

  componentWillReceiveProps(nextProps) {
    let problem = nextProps.params.problem;

    if (nextProps.params.problem !== this.state.problemName) {
      this.setState({
        problemName: problem,
        problem: null
      });

      this.load(problem);
    }
  }

  componentWillUnmount() {
    this.request.abort();
  }

  load(problem) {
    if (this.request) {
      this.request.abort();
    }

    if (!problem) {
      return;
    }

    this.request = $.get(
      '/api/problem/' + problem
    ).done(
      (res) => this.setState({problem: res})
    ).fail(
      () => {} //this.load()
    );
  }

  render() {
    if (!this.state.problem) {
      return null;
    }

    return (
      <row>
        <h1><TeX>{this.state.problem.description}</TeX></h1>
      </row>
    );
  }
}

export default Problem;
