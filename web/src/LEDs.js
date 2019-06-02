import React from 'react';
import './App.css';

class LEDConfig extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            leds: [],
        }
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
                        <tr>
                            <td>{i}</td>
                            <td>{led}</td>
                            <td>Delete</td>
                        </tr>
                    )
                )
                }
                </tbody>

            </table>
        );
    }

}

export default LEDConfig;
