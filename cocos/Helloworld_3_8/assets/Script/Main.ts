import { _decorator, Component } from 'cc';
import FormMgr from './UIFrame/FormMgr';
import UIManager from './UIFrame/UIManager';
import { IFormConfig } from './UIFrame/Struct';
import { UIConfigManager } from './UIConfig';
import CocosHelper from './Common/Utils/CocosHelper';
const {ccclass, property} = _decorator;


@ccclass('Main')
export default class Main extends Component { 
    onLoad() {
        //console.log(this.node.activeInHierarchy); // 必须为true
        //this.node.active = true; // 强制激活
    }
    
    async start () {
        //this.test();
        //let com:any = await  FormMgr.openByName("UINavigator");
        //CocosHelper.TestTransProps(com.node);
        //UINavigator.open();
        //console.log("hello")
    }
    async test():Promise<Boolean>  {
        //let form: IFormConfig= UIConfigManager.getInstance().getFormConfig("UIAbout");
        let form: IFormConfig= UIConfigManager.getInstance().getFormConfig("UINavigator");
        let com = await UIManager.getInstance().openForm(form, null, null);

        this.node.addChild(com.node);

        return true;
    }
    
    onDestroy() {
        console.log("main destory!")
    }

}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import PropController from "./Common/Components/PropController";
// import UINavigator from "./UIScript/UINavigator";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class Main extends cc.Component {
//     
//     @property(PropController) building: PropController = null;
//     onLoad() {
//         
//     }
// 
//     start () {
//         UINavigator.open();
//     }
//     
//     onDestroy() {
//         
//     }
// }
