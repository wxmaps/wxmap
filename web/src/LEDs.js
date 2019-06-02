import React from 'react';
import './App.css';

class LEDConfig extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            leds: [],
        };
        this.addRow = this.addRow.bind(this);
        this.deleteRow = this.deleteRow.bind(this);
        this.editLED = this.editLED.bind(this);
    }

    componentDidMount() {
        this.setState({
            leds: this.props.leds,
        })
    }

    componentWillReceiveProps(nextProps) {
        this.setState({
            leds: nextProps.leds,
        })
    }

    addRow(evt) {
        let array = [...this.state.leds];
        array.push('KMPO');
        this.props.setLEDs(array);
    }

    deleteRow(evt) {
        let array = [...this.state.leds];
        array.splice(evt.target.id, 1);
        this.props.setLEDs(array);
    }

    editLED(evt) {
        let array = [...this.state.leds];
        array[evt.target.id] = evt.target.value;
        this.props.setLEDs(array);
    }

    render() {
        const {leds} = this.state;

        return (
            <table>
                <thead>
                <tr>
                    <th>#</th>
                    <th>Code</th>
                    <th><button type="button" onClick={this.addRow}>+</button></th>
                </tr>
                </thead>

                <tbody>
                {leds.map((led, i) => (
                        <tr key={i}>
                            <td>{i}</td>
                            <td><input type="text" value={led} id={i} onChange={this.editLED} /></td>
                            <td>
                                <button type="button" id={i} onClick={this.deleteRow}>X</button>
                            </td>
                        </tr>
                    )
                )}
                </tbody>

            </table>
        );
    }
}

export default LEDConfig;
