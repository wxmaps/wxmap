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
        };
        this.setLEDs = this.setLEDs.bind(this);
        this.editParam = this.editParam.bind(this);
    }

    componentDidMount() {
        fetch('/config.json')
            .then(res => res.json())
            .then((res) => {
                this.setState({
                    hostname: res.hostname,
                    ssid: res.ssid,
                    passphrase: res.passphrase,
                    leds: res.leds,
                    metarServer: res.metarServer,
                });
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

    render() {
        const {hostname, ssid, passphrase, leds, metarServer} = this.state;

        return (
            <div className="App">
                <header className="App-header">
                    <p>
                        WXMaps Configuration
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

                    <LEDConfig setLEDs={this.setLEDs} leds={leds}/>
                </header>
            </div>
        );
    }
}

export default App;
