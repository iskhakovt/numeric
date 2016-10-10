// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License


import React from 'react'
import { Link } from 'react-router'
import $ from 'jquery-ajax'
import _ from 'lodash'
import { Nav, Navbar, NavItem } from 'react-bootstrap'
import { LinkContainer } from 'react-router-bootstrap'
import DocumentTitle from 'react-document-title'
import TeX from 'react-components/js/tex.jsx'

import csrf from './csrf'
csrf($);


class Problems extends React.Component {
  constructor(props, context) {
    super(props, context);
    this.state = {
      problems: {}
    };
  }

  componentDidMount() {
    this.request = null;
    this.load();
  }

  componentWillUnmount() {
    this.request.abort();
  }

  load() {
    if (this.request) {
      this.request.abort();
    }

    this.request = $.get(
      '/api/problems',
    ).done(
      (res) => this.setState({problems: res})
    ).fail(
      () => this.load()
    );
  }

  render() {
    let sortedKeys = _.sortBy(Object.keys(this.state.problems));

    return (
      <div>
        <DocumentTitle title='numeric'></DocumentTitle>
        <Navbar>
          <Navbar.Header key="brand">
            <Navbar.Brand>
              numeric
            </Navbar.Brand>
          </Navbar.Header>
          <Nav key="problems">
            {
              _.map(
                sortedKeys,
                (key) =>
                  <LinkContainer key={key} to={`/problem/${key}`}>
                    <NavItem key={key} active={true}>
                      <TeX>{this.state.problems[key].short_description}</TeX>
                    </NavItem>
                  </LinkContainer>
              )
            }
          </Nav>
        </Navbar>

        <div className="container">
          {this.props.children}
        </div>
       </div>
    );
  }
}

export default Problems;
