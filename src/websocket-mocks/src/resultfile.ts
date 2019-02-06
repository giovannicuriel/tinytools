class ResultFile{

    private _path: string = "";
    private _newPath: string = "";

    private constructor(path: string, newPath: string){
        this._path = path;
        this._newPath = newPath;
    }

    get path(): string{
        return this._path;
    }

    get newPath(): string{
        return this._newPath;
    }
    
    public static get instance(): ResultFile{
        let date = new Date();
        let day = date.getDate();
        let month = date.getMonth();
        let year = date.getFullYear();
        let hour = date.getHours();
        let minute = date.getMinutes();
        let seconds = date.getSeconds();
        let newName = `${day}_${month}_${year}_${hour}_${minute}_${seconds}`;

        return new ResultFile("/home/uploads/result.csv",`/home/uploads/${newName}.csv`);
    }

}

export { ResultFile }