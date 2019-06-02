
import React from 'react';
import './App.css';
import LEDConfig from './LEDs';

class App extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            hostname: '',
            ssid: '',
            passphrase: '',
            leds: [],
            metarServer: '',
            mode: {},
            message: '',
        };
        this.setLEDs = this.setLEDs.bind(this);
        this.editParam = this.editParam.bind(this);
        this.postConfig = this.postConfig.bind(this);
        this.editModeOptions = this.editModeOptions.bind(this);
    }

    componentDidMount() {
        Promise.all([fetch('/config.json'), fetch('/mode.json')])
            .then(res => {
                Promise.all(res.map(resp => resp.json()))
                    .then(res => {
                        console.log(res);
                        this.setState({
                            hostname: res[0].hostname,
                            ssid: res[0].ssid,
                            passphrase: res[0].passphrase,
                            leds: res[0].leds,
                            metarServer: res[0].metarServer,
                            mode: res[1],
                        })
                    })
            })
    }

    setLEDs(leds) {
        this.setState({
            leds,
        })
    }

    editParam(evt) {
        this.setState({
            [evt.target.name]: evt.target.value,
        })
    }

    editModeOptions(evt) {
        if(evt.target !== undefined) {
            let mode = {...this.state.mode};
            if(evt.target.name === 'duration') {
                mode['config'].duration = evt.target.value;

            } else if(evt.target.name === 'modeId') {
                mode.id = evt.target.value;
            }
            this.setState({
                mode
            })

        }
    }

    postConfig() {
        fetch('/config.json', {method: 'POST', body: JSON.stringify(this.state), headers: {
                'Content-Type': 'application/json'
            }})
            .then(res => res.json())
            .then(res => {this.setState({message: 'POST successful'})})
            .catch(err => {
                this.setState({message: 'Error POSTing to device'})
            });
        fetch('/mode.json', {method: 'POST', body: JSON.stringify(this.state.mode), headers: {
                'Content-Type': 'application/json'
            }})
            .then(res => res.json())
            .then(res => {this.setState({message: 'POST successful'})})
            .catch(err => {
                this.setState({message: 'Error POSTing to device'})
            });
    }

    render() {
        const {hostname, ssid, passphrase, leds, metarServer, message, mode} = this.state;
        console.log(mode);
        return (
            <div className="App">
                <header className="App-header">
                    <p>
                        WXMaps Configuration
                        <br/>
                        {message}
                    </p>
                    <p>
                        <label>Hostname</label>
                        <input value={hostname} onChange={this.editParam} name="hostname" />
                    </p>
                    <p>
                        <label>SSID</label>
                        <input value={ssid} onChange={this.editParam} name="ssid"/>
                    </p>
                    <p>
                        <label>Passphrase</label>
                        <input value={passphrase} onChange={this.editParam} name="passphrase"/>
                    </p>
                    <p>
                        <label>Metar Server</label>
                        <input value={metarServer} onChange={this.editParam} name="metarServer"/>
                    </p>
                    <p>

                        {mode.config === undefined ? (
                            <React.Fragment>
                                <label>Loading...</label>
                            </React.Fragment>
                        ) : (
                            <React.Fragment>
                                <label>Data Type</label>
                                <input value={mode.id} type="number" onChange={this.editModeOptions} name="modeId"/>
                                <br />
                                <label>Duration</label>
                                <input value={mode['config'].duration} type="number" onChange={this.editModeOptions} name="duration"/>
                            </React.Fragment>
                        )}
                    </p>

                    <LEDConfig setLEDs={this.setLEDs} leds={leds}/>
                    <button onClick={this.postConfig}>Save</button>
                </header>
            </div>
        );
    }
}

export default App;
