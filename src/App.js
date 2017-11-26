import React, { Component } from 'react';
import logo from './logo.png';
import './App.css';

import 'whatwg-fetch'

class App extends Component {

  constructor(props) {
    super(props);
    this.state = {bins:{}, updating: 0};
    this.update = this.update.bind(this);
    this.renderBins = this.renderBins.bind(this);
  }

  componentDidMount() {
    console.log("did mount");
    this.update();
  }

  update() {
    console.log("updating");
    this.setState({updating: 1});
    fetch("https://thor.fabianschulz.ch/getBins.php").then(response => {
          console.log(response);
          return response.json();
        })
          .then(json => {
            console.log(json);
            this.setState({bins: json, updating: 0});
            setTimeout(this.update, 5000);
        })
  }

  renderBins() {
    let binRender = [];
    for(let bin in this.state.bins) {
      console.log("bin: "+bin);
      binRender.push(
        <div key={this.state.bins[bin][0]} className="bin">{this.state.bins[bin][0]+" has "+this.state.bins[bin][1]+" cm space and the battery is at "+this.state.bins[bin][2]/100+" volts. Last update: "+this.state.bins[bin][3]}</div>
      )
    }
    return binRender;
  }

  render() {
    return (
      <div className="App">
        <header className="App-header">
          <img src={logo} className="App-logo" alt="logo" />
          <h1 className="App-title">Welcome to the Thor Trash bin Manager</h1>
        </header>
        <p className="App-intro">
          {this.renderBins()}
        </p>
        {this.state.updating?"updating":null}
      </div>
    );
  }
}

export default App;
