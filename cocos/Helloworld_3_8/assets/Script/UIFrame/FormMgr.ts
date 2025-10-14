import { error,warn } from "cc";
import { FormType } from "./config/SysDefine";
import FixedMgr from "./FixedMgr";
import SceneMgr from "./SceneMgr";
import { IFormConfig, IFormData } from "./Struct";
import TipsMgr from "./TipsMgr";
import ToastMgr from "./ToastMgr";
import UIManager from "./UIManager";
import WindowMgr from "./WindowMgr";
import { UIConfigManager } from "../UIConfig";
import { IUIBase } from "./IUIHandler";

class FormMgr {
    /**
     * 打开窗体
     * @param form 窗体配置信息
     * @param param 自定义参数
     * @param formData 窗体处理时的一些数据
     */
    async open(form: IFormConfig, param?: any, formData?: IFormData):Promise<IUIBase> {
        switch(form.type) {
            case FormType.Screen:
                return await SceneMgr.open(form, param, formData);
            case FormType.Window:
                return await WindowMgr.open(form, param, formData);
            case FormType.Fixed:
                return await FixedMgr.open(form, param, formData);
            case FormType.Tips:
                return await TipsMgr.open(form, param, formData);
            case FormType.Toast:
                return await ToastMgr.open(form, param, formData);
            default:
                error(`未知类型的窗体: ${form.type}`);
                return null;
        }
    }
    async openByName(uiname:string, param?: any, formData?: IFormData):Promise<IUIBase> {
        let form: IFormConfig = UIConfigManager.getInstance().getFormConfig(uiname);
        
        return this.open(form,param,formData);
    }

    async close(form: IFormConfig, param?: any, formData?: IFormData) {
        switch(form.type) {
            case FormType.Screen:
                return await SceneMgr.close(form, param, formData);
            case FormType.Window:
                return await WindowMgr.close(form, param, formData);
            case FormType.Fixed:
                return await FixedMgr.close(form, param, formData);
            case FormType.Tips:
                return await TipsMgr.close(form, param, formData);
            case FormType.Toast:
                warn("UIToast 目前不能通过这种方式关闭, 请使用 ToastMgr.close()");
                break;
            default:
                error(`未知类型的窗体: ${form.type}`);
                return false;
        }
    }

    async backScene(params?: any, formData?: IFormData) {
        return SceneMgr.back(params, formData);
    }

    async closeAllWindows() {
        await WindowMgr.closeAll();
    }

    async load(form: IFormConfig) {
        await UIManager.getInstance().loadUIForm(form);
    }

    async loadByname(name:string):Promise<IUIBase> {
        let form: IFormConfig = UIConfigManager.getInstance().getFormConfig(name);
        if (form){
            await UIManager.getInstance().loadUIForm(form);
        }
        return null;
    }
}

export default new FormMgr();