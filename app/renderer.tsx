
import * as ReactHotLoader from "react-hot-loader";
import * as React from "react";
import * as ReactDOM from "react-dom";

import "./index.css";

const hot_loader = ReactHotLoader.hot(module);

class TitleBar extends React.Component<{ visible: boolean; }, unknown>
{
    constructor(props: { visible: boolean; })
    {
        super(props);
    }
    render(): JSX.Element
    {
        if (this.props.visible)
            return <div className="title-bar"></div>;
        else
            return <div className="title-bar hidden"></div>;
    }
}

class MessageForm extends React.Component<unknown, { value: string; }>
{
    constructor(props: unknown)
    {
        super(props);
        this.state = { value: "" };

        this.onChange = this.onChange.bind(this);
        this.onSubmit = this.onSubmit.bind(this);
    }
    onChange(event: React.ChangeEvent<HTMLInputElement>): void
    {
        this.setState({ value: event.target.value });
    }
    onSubmit(event: React.FormEvent<HTMLFormElement>): void
    {
        window.Electron.ipcRenderer.send("message-form", this.state.value);
        this.setState({ value: "" });
        event.preventDefault();
    }
    render(): JSX.Element
    {
        const element =
            <form onSubmit={this.onSubmit}>
                <input type="text" value={this.state.value} onChange={this.onChange} />
                <br />
                <input type="submit" value="send" />
            </form>;
        return element;
    }
}

class Main extends React.Component<unknown, { full_screen: boolean; }>
{
    constructor(props: unknown)
    {
        super(props);
        this.state = { full_screen: false };

        this.onFullScreen = this.onFullScreen.bind(this);
        window.Electron.ipcRenderer.on("full-screen", this.onFullScreen);
    }
    onFullScreen(): void
    {
        this.setState({ full_screen: !this.state.full_screen });
    }
    render(): JSX.Element
    {
        const element =
            <>
                <TitleBar visible={!this.state.full_screen} />
                <MessageForm />
            </>;
        return element;
    }
}

export default hot_loader(Main);
ReactDOM.render(<Main />, document.querySelector("#root"));