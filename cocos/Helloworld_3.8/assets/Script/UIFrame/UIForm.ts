// // @ts-ignore
// // @ts-ignore
// // @ts-ignore
// // @ts-ignore
// // @ts-ignore

import { _decorator,tween,easing, Vec3 } from 'cc';
import { IPool } from "../Common/Utils/Pool";
import { FormType, ModalOpacity } from "./config/SysDefine";
import FormMgr from "./FormMgr";
import { ECloseType, ModalType } from "./Struct";
import UIBase from './UIBase';
import CocosHelper from '../Common/Utils/CocosHelper';
// cc.UIScreen = UIScreen;
// cc.UIWindow = UIWindow;
// cc.UIFixed = UIFixed;
// cc.UITips = UITips;
// cc.UIToast = UIToast;

export class UIScreen extends UIBase{
    formType = FormType.Screen;
    closeType = ECloseType.CloseAndDestory;
}

export class UIWindow extends UIBase {
    formType = FormType.Window;    
    modalType = new ModalType(ModalOpacity.OpacityFull);                // 阴影类型
    closeType = ECloseType.LRU;
//    /** 显示效果 */
    public async showEffect() {
        //this.node.scale = 0;
        this.node.setScale(0,0,0);// 设置所有轴的缩放为0
        //import { easing } from 'cc';
        //easing.backOut
        //如果你还在使用旧项目升级到 3.8，建议检查所有 cc.easeXXX 的调用并替换为新的 easing.xxx 形式。
        // .to(1, { position: new Vec3(100, 100, 0) }, { easing: easing.backOut })
        //await CocosHelper.runTweenSync(this.node, tween().to(1, { position: new Vec3(100, 100, 0) }, { easing: easing.backOut }));
        await CocosHelper.runTweenSync(this.node, tween().to(0.3, { scale:1}, { easing: easing.backOut }));
        
        //await CocosHelper.runTweenSync(this.node, tween().to(0.3, {scale: 1}, easing.backOut  ));
    }
}

export class UIFixed extends UIBase {
    formType = FormType.Fixed;
    closeType = ECloseType.LRU;
}

export class UITips extends UIBase {
    formType = FormType.Tips;
    closeType = ECloseType.CloseAndHide;
}

export class UIToast extends UIBase implements IPool {
    formType = FormType.Toast;
    public use() {

    }
    public free() {

    }
    public async closeSelf(): Promise<boolean> {
        //return await FormMgr.close({prefabUrl: this.fid, type: this.formType});
        return await FormMgr.close(this.formData.loadingForm);
    }
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import { IPool } from "../Common/Utils/Pool";
// import CocosHelper from "./CocosHelper";
// import { FormType, ModalOpacity } from "./config/SysDefine";
// import FormMgr from "./FormMgr";
// import { ECloseType, ModalType } from "./Struct";
// import UIBase from "./UIBase";
// 
// 
// export class UIScreen extends UIBase {
//     formType = FormType.Screen;
//     closeType = ECloseType.CloseAndDestory;
// }
// 
// export class UIWindow extends UIBase {
//     formType = FormType.Window;    
//     modalType = new ModalType(ModalOpacity.OpacityFull);                // 阴影类型
//     closeType = ECloseType.LRU;
// 
//     /** 显示效果 */
//     public async showEffect() {
//         this.node.scale = 0;
//         await CocosHelper.runTweenSync(this.node, cc.tween().to(0.3, {scale: 1}, cc.easeBackOut()));
//     }
// }
// 
// export class UIFixed extends UIBase {
//     formType = FormType.Fixed;
//     closeType = ECloseType.LRU;
// }
// 
// export class UITips extends UIBase {
//     formType = FormType.Tips;
//     closeType = ECloseType.CloseAndHide;
// }
// 
// export class UIToast extends UIBase implements IPool {
//     formType = FormType.Toast;
// 
//     public use() {
//         
//     }
// 
//     public free() {
// 
//     }
// 
//     public async closeSelf(): Promise<boolean> {
//         return await FormMgr.close({prefabUrl: this.fid, type: this.formType});
//     }
// }
// 
// 
// // @ts-ignore
// cc.UIScreen = UIScreen;
// // @ts-ignore
// cc.UIWindow = UIWindow;
// // @ts-ignore
// cc.UIFixed = UIFixed;
// // @ts-ignore
// cc.UITips = UITips;
// // @ts-ignore
// cc.UIToast = UIToast;
