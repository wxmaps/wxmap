
import React from 'react';
import { withStyles } from '@material-ui/core/styles';
import LEDConfig from './LEDs';
import AppBar from '@material-ui/core/AppBar';
import Container from '@material-ui/core/Container';
import CircularProgress from '@material-ui/core/CircularProgress';
import CssBaseline from '@material-ui/core/CssBaseline';
import Paper from '@material-ui/core/Paper';
import TextField from '@material-ui/core/TextField';
import Button from '@material-ui/core/Button';

const classes = {
    root: {
        flexGrow: 1,
    },
    paper: {
        paddingBottom: 50,
    },
};

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
        return (
            <Container className={classes.root}>
                <CssBaseline />
                    <AppBar position="static" color="primary">
                        <p>
                            WXMaps Configuration
                            <br/>
                            {message}
                        </p>
                        <Paper className={classes.paper}>
                            <TextField value={hostname} label="Hostname" onChange={this.editParam} name="hostname" />
                            <TextField value={ssid} label="SSID" onChange={this.editParam} name="ssid"/>
                            <TextField value={passphrase} label="Passphrase" onChange={this.editParam} name="passphrase"/>
                            <TextField value={metarServer} label="METAR Server" onChange={this.editParam} name="metarServer"/>
                            {mode.config === undefined ? (
                                <React.Fragment>
                                    <CircularProgress />
                                </React.Fragment>
                            ) : (
                                <React.Fragment>
                                    <TextField value={mode.id} label="Mode ID" type="number" onChange={this.editModeOptions} name="modeId"/>
                                    <br />
                                    <TextField value={mode['config'].duration} label="Duration" type="number" onChange={this.editModeOptions} name="duration"/>
                                </React.Fragment>
                            )}
                        </Paper>

                        <Paper className={classes.paper}>
                            <LEDConfig setLEDs={this.setLEDs} leds={leds}/>


                        </Paper>
                        <Button onClick={this.postConfig}>Save</Button>
                    </AppBar>
            </Container>
        );
    }
}

export default withStyles(classes)(App);
