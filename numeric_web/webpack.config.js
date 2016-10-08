var path = require("path");
var webpack = require('webpack');
var BundleTracker = require('webpack-bundle-tracker');

module.exports = {
  context: __dirname,

  // entry point of our app. assets/js/index.js should require other js modules and dependencies it needs
  entry: './assets/js/index.jsx',

  devtool: 'source-map',

  output: {
    path: path.resolve('./assets/bundles/'),
    filename: '[name]-[hash].js'
  },

  plugins: [
    new BundleTracker({filename: './webpack-stats.json'})
  ],

  module: {
    loaders: [
      // To transform JSX into JS
      {test: /\.(jsx)?$/, exclude: /node_modules/, loader: 'babel-loader', query: {presets: ['es2015', 'react']}}
    ],
    preLoaders: [
      { test: /\.(js|jsx)?$/, loader: 'standard', exclude: /(node_modules|bower_components)/}
    ],
    standard: {
      // Options to be passed through to standard
      parser: 'babel-eslint'
    }
  },

  resolve: {
    modulesDirectories: ['node_modules', 'bower_components'],
    extensions: ['', '.js', '.jsx']
  }
};
