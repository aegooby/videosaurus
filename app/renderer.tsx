
import * as ReactHotLoader from "react-hot-loader";
import * as React from "react";
import * as ReactDOM from "react-dom";

import "./index.css";

class TitleBar extends React.Component<{ visible: boolean; }, unknown>
{
    constructor(props: { visible: boolean; })
    {
        super(props);
    }
    render(): React.ReactElement
    {
        if (this.props.visible)
            return <div className="title-bar"></div>;
        else
            return <div className="title-bar hidden"></div>;
    }
}
type ConnectionType = "none" | "host" | "client";
class MessageForm
    extends React.Component<{ ip: string; connectionType: ConnectionType; },
    { message: string; history: string; }>
{
    constructor(props: { ip: string; connectionType: ConnectionType; })
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
class NetworkInterface extends React.Component<unknown,
    { address: string; connected: boolean; connectionType: ConnectionType; }>
{
    constructor(props: unknown)
    {
        super(props);

        this.state = { address: "", connected: false, connectionType: "none" };

        this.onAddressChange = this.onAddressChange.bind(this);
        this.onHostSubmit = this.onHostSubmit.bind(this);
        this.onClientSubmit = this.onClientSubmit.bind(this);
        this.onHostButtonClick = this.onHostButtonClick.bind(this);
        this.onClientButtonClick = this.onClientButtonClick.bind(this);
        this.onCreateNode = this.onCreateNode.bind(this);

        window.Electron.ipcRenderer.removeAllListeners("create-node");
        window.Electron.ipcRenderer.on("create-node", this.onCreateNode);
    }
    onAddressChange(event: React.ChangeEvent<HTMLInputElement>): void
    {
        this.setState({ address: event.target.value });
    }
    onHostSubmit(event: React.FormEvent<HTMLFormElement>): void
    {
        window.Electron.ipcRenderer.send("create-node", true, "*");
        this.setState({ connected: true });
        event.preventDefault();
    }
    onClientSubmit(event: React.FormEvent<HTMLFormElement>): void
    {
        const address = this.state.address.trim();
        if (address === "")
        {
            event.preventDefault();
            return;
        }
        if (address === "localhost")
            window.Electron.ipcRenderer.send("create-node", false, "127.0.0.1");
        else
            window.Electron.ipcRenderer.send("create-node", false, address);
        this.setState({ connected: true });
        event.preventDefault();
    }
    onHostButtonClick(): void
    {
        this.setState({ connectionType: "host" });
    }
    onClientButtonClick(): void
    {
        this.setState({ connectionType: "client" });
    }
    onCreateNode(_: unknown, address: string): void
    {
        this.setState({ address: address });
    }
    render(): React.ReactElement
    {
        let subElement: React.ReactElement;
        switch (this.state.connectionType)
        {
            case "none":
                subElement =
                    <>
                        <button type="button" onClick={this.onHostButtonClick}>
                            host
                        </button>
                        <br />
                        <button type="button" onClick={this.onClientButtonClick}>
                            client
                        </button>
                    </>;
                break;
            case "host":
                subElement =
                    <form onSubmit={this.onHostSubmit}>
                        <input type="submit" value="create" />
                    </form>;
                break;
            case "client":
                subElement =
                    <form onSubmit={this.onClientSubmit}>
                        <input
                            type="text" className="monospace"
                            value={this.state.address}
                            onChange={this.onAddressChange}
                            placeholder="IP address" />
                        <br />
                        <input type="submit" value="join" />
                    </form>;
                break;
        }
        if (this.state.connected)
            subElement = <MessageForm
                ip={this.state.address}
                connectionType={this.state.connectionType} />;
        const element = <div className="form-options">{subElement}</div>;
        return element;
    }
}

class Main extends React.Component<unknown, { fullScreen: boolean; }>
{
    constructor(props: unknown)
    {
        super(props);
        this.state = { fullScreen: false };

        this.onFullScreen = this.onFullScreen.bind(this);

        window.Electron.ipcRenderer.removeAllListeners("full-screen");
        window.Electron.ipcRenderer.on("full-screen", this.onFullScreen);
    }
    onFullScreen(): void
    {
        this.setState({ fullScreen: !this.state.fullScreen });
    }

    render(): React.ReactElement
    {
        const element =
            <>
                <TitleBar visible={!this.state.fullScreen} />
                <NetworkInterface />
            </>;
        return element;
    }
}

const hotLoader = ReactHotLoader.hot(module);
export default hotLoader(Main);

ReactDOM.render(<Main />, document.querySelector("#root"));