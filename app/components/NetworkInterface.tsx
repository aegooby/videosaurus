
import * as React from "react";

import { MessageForm } from "./MessageForm";

type ConnectionType = "none" | "host" | "client";

interface State
{
    address: string;
    connected: boolean;
    connectionType: ConnectionType;
}

export class NetworkInterface extends React.Component<unknown, State>
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