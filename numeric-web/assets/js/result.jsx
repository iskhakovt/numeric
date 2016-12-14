// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License


import React from 'react'
import $ from 'jquery-ajax'
import _ from 'lodash'
import Baby from 'babyparse'
import { Grid, Row, Alert, Well, Button, Label } from 'react-bootstrap'
import DocumentTitle from 'react-document-title'
import Plotly from 'react-plotlyjs'
import TeX from 'react-components/js/tex.jsx'
import {saveAs} from 'file-saver'

import csrf from './csrf'
csrf($);


class Result extends React.Component {
  constructor(props, context) {
    super(props, context);
    this.state = {
      id: null
    };
  }

  componentDidMount() {
    this.request = null;
    this.load(this.props.params.id)
  }

  componentWillReceiveProps(nextProps) {
    let id = nextProps.params.id;
    if (id !== this.state.id) {
      this.load(id);
    }
  }

  componentWillUnmount() {
    if (this.request) {
      this.request.abort();
    }
  }

  load(id) {
    if (this.request) {
      this.request.abort();
    }

    this.setState({
      id: id,
      status: 'PE'
    });

    $.get(
      '/api/result/' + id
    ).done(
      (res) => {
        this.setState({
          status: res.status,
          result: res.result,
          time: res.time,
          args: res.args,
        });

        if (res.status === 'PE') {
          setTimeout(() => this.load(id), 500);
        }
      }
    ).fail(
      () => this.load(id)
    );
  }

  download(filename, data) {
    var array = [];

    for (var i = 0; i != data[0].length; ++i) {
      array.push([data[0][i], data[1][i]]);
    }

    let csvContent = Baby.unparse(array);
    saveAs(
      new Blob(
        [csvContent],
        {type: 'text/csv;charset=US-ASCII'}
      ),
      filename + '.csv',
      true
    );
  }

  getResult(data, key) {
    if (data.type === 'title') {
      return (
        <p key={key}>
          <h1><TeX>{data.data}</TeX></h1>
          <hr />
        </p>
      );
    } else if (data.type === 'text') {
      return (
        <h4 key={key}><TeX>{data.data}</TeX></h4>
      );
    } else if (data.type === 'function') {
      return (
        <p>
          <Button
            bsStyle="info"
            onClick={() => this.download(data.filename, data.data)}
            key={key}
          >
            <TeX>{data.description}</TeX>
          </Button>
        </p>
      );
    } else if (data.type === 'graph') {
      var graphData = [
        {
          type: 'scatter',
          x: data.data[0],
          y: data.data[1],
          marker: {
            color: 'rgb(16, 32, 77)'
          }
        },
      ];

      let layout = {
        title: data.description
      }

      return (
        <p>
          <Plotly
            className="whatever"
            data={graphData}
            layout={layout}
            key={key}
          />
        </p>
      );
    }
  }

  render_() {
    if (this.state.id === null) {
      return null;
    }

    if (this.state.status === 'PE') {
      return (
        <h2>Производятся вычисления...</h2>
      );
    }

    if (this.state.status === 'FL') {
      return (
        <Alert bsStyle="danger">
          <h4>Ошибка</h4>
          <Well>{this.state.result.error}</Well>
        </Alert>
      );
    }

    return (
      <Grid>
        <Row>
          <Label bsStyle="success">
            {'(' + this.state.time.toFixed(2) + ' seconds)'}
          </Label>
          <DocumentTitle title='Результаты'></DocumentTitle>
          {
            _.map(this.state.result, (res, key) => this.getResult(res, key))
          }
        </Row>
      </Grid>
    );
  }

  render() {
    return (
      <div>
        <DocumentTitle title='Результаты'></DocumentTitle>
        {this.render_()}
      </div>
    )
  }
}

export default Result;
