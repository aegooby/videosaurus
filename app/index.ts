
import * as electron from "electron";
import * as path from "path";
import * as fs from "fs";
import * as jszip from "jszip";

/* eslint-disable-next-line @typescript-eslint/ban-ts-comment */
/* @ts-ignore */
import * as native from "../engine/build/Release/videosaurus.node";
import { fstat } from "fs";


declare const MAIN_WINDOW_WEBPACK_ENTRY: string;

if (require("electron-squirrel-startup"))
    electron.app.quit();

process.env["ELECTRON_DISABLE_SECURITY_WARNINGS"] = "true";

const icon_path = path.join(__dirname, "icon.png");
const icon_image = electron.nativeImage.createFromPath(icon_path);
icon_image.isMacTemplateImage = true;
electron.app.dock.setIcon(icon_image);

const extensions_dir = path.join(__dirname, "../../extensions");
const devtools_zip = path.join(extensions_dir, "react-devtools.zip");
const devtools_dir = path.join(extensions_dir, "react-devtools");

async function unzip_devtools(): Promise<void>
{
    const buffer = await fs.promises.readFile(path.resolve(devtools_zip));
    const zip = await jszip.loadAsync(buffer);
    const keys = Object.keys(zip.files);
    async function unzip(filename: string): Promise<void>
    {
        console.log(filename);
        const is_file = !zip.files[filename].dir;
        const full_path = path.join(extensions_dir, filename);
        const directory = is_file && path.dirname(full_path) || full_path;
        const content = await zip.files[filename].async("nodebuffer");

        await fs.promises.mkdir(directory, { recursive: true });
        if (is_file)
            await fs.promises.writeFile(full_path, content);
    }
    keys.map(unzip);
}

async function ready(): Promise<void>
{
    try
    {
        // console.log(native.add(5, 10));
        await fs.promises.access(devtools_dir);
    }
    catch (error) 
    {
        await unzip_devtools();
    }
    try
    {
        create_window();
        await electron.session.defaultSession.loadExtension(devtools_dir);
    }
    catch (error) { console.log(error); }
}

function create_window(): void
{
    const window_preferences =
    {
        width: 800,
        height: 600,
        minHeight: 600,
        minWidth: 800,
        frame: false,
        titleBarStyle: "hiddenInset" as const,
        webContents: { contextIsolation: true },
        icon: icon_image,
    };

    const main_window = new electron.BrowserWindow(window_preferences);
    main_window.loadURL(MAIN_WINDOW_WEBPACK_ENTRY);
}

electron.app.on("ready", ready);

function window_all_closed(): void
{
    if (process.platform !== "darwin")
        electron.app.quit();
}
electron.app.on("window-all-closed", window_all_closed);

function activate(): void
{
    if (electron.BrowserWindow.getAllWindows().length === 0)
        create_window();
}
electron.app.on("activate", activate);
