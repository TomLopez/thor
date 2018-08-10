import React, { Component } from 'react';
import logo from './logo.png';
import thorPetLogo from './thorPetLogo.png'
import thorPetLogo2 from './thorPet2.png'
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
    let proxyUrl = 'https://cors-anywhere.herokuapp.com/',
        targetUrl = 'https://thor.fabianschulz.ch/getBins.php';
      fetch(targetUrl).then(response => {
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
    let i=0;
    for(let bin in this.state.bins) {
      console.log("bin: "+bin);
      binRender.push(
        <div key={this.state.bins[bin][0]} className="bin">
            {i%2===0?<img src={thorPetLogo} className="Bin-logo" alt="logo" />:<img src={thorPetLogo2} className="Bin-logo" alt="logo" />}
            <div className="binTitle">
                {this.state.bins[bin][0]}
            </div>
            <div className="binElement spaceLeft">
                <div className="elementContainer"><div className="leftDiv">{"Full at: "}</div><div className="rightDiv">{this.state.bins[bin][1] + " %"}</div></div>
            </div>
            <div className="binElement batteryVoltage">
                <div className="elementContainer"><div className="leftDiv">{"Battery voltage: "}</div><div className="rightDiv">{this.state.bins[bin][2]/10 + " V"}</div></div>
            </div>
            <div className="binElement bottlesCrushed">
                <div className="elementContainer"><div className="leftDiv">{"Total bottles crushed: "}</div><div className="rightDiv">{this.state.bins[bin][3]}</div></div>
            </div>
            <div className="binElement timestamp">
                <div className="elementContainer"><div className="leftDiv">{"Last update received: "}</div><div className="rightDiv">{this.state.bins[bin][4]}</div></div>
            </div>
        </div>
      );
        i++;
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
        <div className="AppContainer">
          {this.renderBins()}
        </div>
        {this.state.updating?"updating":null}
      </div>
    );
  }
}

export default App;
