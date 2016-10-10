// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License


import React from 'react'
import { withRouter } from 'react-router'
import $ from 'jquery-ajax'
import _ from 'lodash'
import Baby from 'babyparse'
import { Grid, Row, Col, Button, Form, FormGroup, ControlLabel, FormControl, HelpBlock } from 'react-bootstrap'
import DocumentTitle from 'react-document-title'
import TeX from 'react-components/js/tex.jsx'

import csrf from './csrf'
csrf($);


class Problem_ extends React.Component {
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

    this.setState({
      problemName: problem,
      problem: null
    });

    this.request = $.get(
      '/api/problem/' + problem
    ).done(
      (res) => this.initProblem(res)
    ).fail(
      () => this.load()
    );
  }

  initProblem(res) {
    this.setState(_.mapValues(res.args, () => ''));
    this.setState({
      problem: res,
      badSubmit: false
    });
  }

  setStateByKey(key, value) {
      var stateObject = {};
      stateObject[key] = value;
      this.setState(stateObject);
  }

  processCSV(argName, e) {
    let parsed = Baby.parse(e.target.result);

    if (parsed.errors.length === 0) {
      this.setStateByKey(argName, parsed.data);
    } else {
      this.setStateByKey(argName, 'error');
    }
  }

  handleChange(e) {
    let argName = e.target.id;
    this.setState({badSubmit: false});

    if (e.target.type === 'file') {
      var reader = new FileReader();
      reader.onload = (e_) => this.processCSV(argName, e_);
      reader.readAsText(e.target.files[0]);
    } else {
      this.setStateByKey(argName, e.target.value);
    }
  }

  getValidationState(argName) {
    let value = this.state[argName];

    if (value === '') {
      return null;
    }

    if (this.state.problem.args[argName].isFunction) {
      if (value !== 'error') {
        return 'success';
      } else {
        return 'error'
      }
    } else {
      if (value.match(/^-?\d*(\.\d+)?$/) && !isNaN(parseFloat(value))) {
        return 'success';
      } else {
        return 'error';
      }
    }
  }

  getForm(argName) {
    var arg = this.state.problem.args[argName];

    if (arg.isFunction) {
      return (
        <FormGroup
          controlId={argName}
          key={argName}
          validationState={this.getValidationState(argName)}
        >
          <ControlLabel><TeX>{arg.description}</TeX></ControlLabel>
          <FormControl
            type="file"
            accept=".csv"
            onChange={(e) => this.handleChange(e)}
          />
          <HelpBlock>Select a function csv file</HelpBlock>
        </FormGroup>
      );
    } else {
      return (
        <FormGroup
          controlId={argName}
          key={argName}
          validationState={this.getValidationState(argName)}
        >
          <ControlLabel><TeX>{arg.description}</TeX></ControlLabel>
          <FormControl
            type="text"
            value={this.state[argName]}
            onChange={(e) => this.handleChange(e)}
          />
        </FormGroup>
      );
    }
  }

  submit() {
    if (!_.map(this.state.problem.args, (arg, argName) =>
        this.getValidationState(argName)).every((x) => x === 'success')) {
      this.setState({badSubmit: true});
      return;
    }

    let problemName = this.state.problemName;
    $.post(
      '/api/query/' + problemName + '/',
      _.mapValues(this.state.problem.args, (arg, argName) => this.state[argName])
    ).done(
      (res) => this.props.router.push('/result/' + res.query)
    )
  }

  render() {
    if (!this.state.problem) {
      return null;
    }

    let sortedArgKeys = _.sortBy(Object.keys(this.state.problem.args));

    return (
      <Grid>
        <Row>
          <DocumentTitle title='Задача'></DocumentTitle>
          <h1><TeX>{this.state.problem.description}</TeX></h1>
          <Col md={6}>
            <Form>
              {
                _.map(sortedArgKeys, (key) => this.getForm(key))
              }
              <Button
                bsStyle={this.state.badSubmit ? 'danger' : null}
                bsSize="large"
                onClick={() => this.submit()}
                block
              >
                Вычислить
              </Button>
            </Form>
          </Col>
        </Row>
      </Grid>
    );
  }
}

var Problem = withRouter(Problem_);

Problem_.propTypes = {
  router: React.PropTypes.shape({
    push: React.PropTypes.func.isRequired
  }).isRequired
};

export default Problem;
