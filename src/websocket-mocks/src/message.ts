class Message{

    private _content: string = "";
    private _last: boolean = false;

    private constructor(content: string, last: boolean){
        this._content = content;
        this._last = last;
    }

    get content(): string{
        return this._content;
    }

    get last(): boolean{
        return this._last;
    }

    public static instance(data: any): Message{
        let currentTime = new Date().getTime();
        let content = `${data.attrs.perf};${data.metadata.timestamp};${currentTime};${data.attrs.total_messages};${data.attrs.last_message}\n`;
        let last = data.attrs.last_message;
        return new Message(content, last);
    }
}

export { Message }