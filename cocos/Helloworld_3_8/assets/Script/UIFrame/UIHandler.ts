
import AdapterMgr from "./AdapterMgr";
import ResMgr from "./ResMgr";
import FormMgr from "./FormMgr";
import { IFormData } from "./Struct";
import { IUIHandler } from "./IUIHandler";

export class UIHandler implements IUIHandler  {
    public async openForm(uiconfig:any,param?: any, formData?: IFormData):Promise<any> {
        return FormMgr.open(uiconfig, param, formData);
    }
    public async closeForm(uiconfig:any):Promise<boolean>{
        return FormMgr.close(uiconfig);
    }

}

//let uihandler:IUIHandler = new UIHandler();
//export default uihandler;
