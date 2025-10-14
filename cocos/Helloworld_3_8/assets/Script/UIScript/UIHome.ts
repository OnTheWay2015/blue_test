import { _decorator } from 'cc';
const {ccclass, property} = _decorator;

//import UIHome_Auto from "../AutoScripts/UIHome_Auto";
import FormMgr from "../UIFrame/FormMgr";
import { UIFixed, UIScreen } from "../UIFrame/UIForm";
import UIManager from "../UIFrame/UIManager";
import CocosHelper from '../Common/Utils/CocosHelper';

@ccclass('UIHome')
export default class UIHome extends UIScreen {
    //public view: UIHome_Auto;
    async load() {
        let com = await FormMgr.openByName("UISound");;
        //await  FormMgr.loadByname("UISound");
        return "";
    }
    model = 5;
    refreshView() {
        // console.log(this.model)
    }
    start () {
        // this.view.Start.addClick(() => {
        // FormMgr.open(UIConfig.UIMap);
        // }, this);

        // this.view.About.addClick(() => {
        // FormMgr.open(UIConfig.UIAbout);
        // }, this);

        // this.view.Back.addClick(() => {
        // FormMgr.backScene();
        // }, this);
    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import UIHome_Auto from "../AutoScripts/UIHome_Auto";
// import FormMgr from "../UIFrame/FormMgr";
// import { UIFixed, UIScreen } from "../UIFrame/UIForm";
// import UIManager from "../UIFrame/UIManager";
// import UIConfig from "./../UIConfig";
// 
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UIHome extends UIScreen {
// 
//     public view: UIHome_Auto;
//     async load() {
//         await UIManager.getInstance().openForm(UIConfig.UISound);;
//         return null;
//     }
// 
//     model = 5;
//     refreshView() {
//         console.log(this.model)
//     }
// 
//     start () {
//         this.view.Start.addClick(() => {
//             FormMgr.open(UIConfig.UIMap);
//         }, this);
// 
//         this.view.About.addClick(() => {
//             FormMgr.open(UIConfig.UIAbout);
//         }, this);
// 
//         this.view.Back.addClick(() => {
//             FormMgr.backScene();
//         }, this);
//     }
// 
//     // update (dt) {}
// }
