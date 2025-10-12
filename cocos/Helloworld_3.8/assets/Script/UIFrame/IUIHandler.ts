import { Asset } from "cc";
import { ECloseType, IFormData } from "./Struct";
import { FormType } from "./config/SysDefine";

export interface IUIHandler {
    openForm(uiconfig:any,param?: any, formData?: IFormData):Promise<any>;
    closeForm(uiconfig:any):Promise<boolean>;
}


export interface IUIBase {
    fid:string
    //node:Node
    node:any
    formData: IFormData
    formType: FormType
    closeType: ECloseType 
    _preInit(params: any) 
    load(params: any): Promise<string> 
    onInit(params: any) 
//    // 显示回调
    onShow(params: any) 
//    // 在显示动画结束后回调
    onAfterShow(params: any) 
//    // 隐藏回调
    onHide(params: any)     
//    // 在隐藏动画结束后回调
    onAfterHide(params: any) 
//    // 关闭自己
    closeSelf(params?: any): Promise<boolean> 
//     * 弹窗动画
//     */
    showEffect() 
    hideEffect() 
//    /** 设置是否挡住触摸事件 */
    setBlockInput(block: boolean) 
    loadRes(url: string, type?: typeof Asset)
}

