
import * as React from "react";

type ConnectionType = "none" | "host" | "client";

interface Props
{
    ip: string;
    connectionType: ConnectionType;
}
interface State
{
    message: string;
    history: string;
}

export class MessageForm extends React.Component<Props, State>
{
    constructor(props: Props)
    {
        super(props);
        this.state = { message: "", history: "" };

        this.onChange = this.onChange.bind(this);
        this.onSubmit = this.onSubmit.bind(this);
    }
    onChange(event: React.ChangeEvent<HTMLInputElement>): void
    {
        this.setState({ message: event.target.value });
    }
    onSubmit(event: React.FormEvent<HTMLFormElement>): void
    {
        if (this.state.message === "")
        {
            event.preventDefault();
            return;
        }
        const host = (this.props.connectionType != "host") ? false : true;
        window.Electron.ipcRenderer.send("send-message", host, this.state.message);
        this.setState({ message: "", history: this.state.history + "\r\n" + this.state.message });
        event.preventDefault();
    }
    render(): React.ReactElement
    {
        const element =
            <>
                <div className="room-info">{"host: " + this.props.ip}</div>
                <br />
                <div className="messages">
                    {this.state.history}
                </div>
                <br />
                <form onSubmit={this.onSubmit}>
                    <input
                        type="text" value={this.state.message}
                        onChange={this.onChange} />
                    <br />
                    <input type="submit" value="send" />
                </form>
            </>;
        return element;
    }
}