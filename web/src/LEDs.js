import React from 'react';
import './App.css';

class LEDConfig extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            leds: [],
        };
        this.deleteRow = this.deleteRow.bind(this);
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

    deleteRow(evt) {
        let array = [...this.state.leds];
        array.splice(evt.target.id, 1);
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
                    <th>Delete</th>
                </tr>
                </thead>

                <tbody>
                {leds.map((led, i) => (
                        <tr key={i}>
                            <td>{i}</td>
                            <td>{led}</td>
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
