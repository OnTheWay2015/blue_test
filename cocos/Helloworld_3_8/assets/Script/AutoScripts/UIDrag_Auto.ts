import { _decorator, Component, Sprite } from 'cc';
const {ccclass, property} = _decorator;

import ButtonPlus from "./../Common/Components/ButtonPlus"

@ccclass('UIDragAuto')
export default class UIDrag_Auto extends Component {
    @property(ButtonPlus)
    about_up: ButtonPlus = null;
    close: ButtonPlus = null;
    abount:Sprite | null = null;
 
    onLoad () {
//        //let v = this.getComponent("abount");

        // let btnNode = this.node.getChildByName("close");
        // this.close = btnNode.getComponent(ButtonPlus);


        // let  sNode = this.node.getChildByName("abount");
        // this.abount = sNode.getComponent(cc.Sprite);


     }
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import ButtonPlus from "./../Common/Components/ButtonPlus"
//  
// const {ccclass, property} = cc._decorator;
// @ccclass
// export default class UIDrag_Auto extends cc.Component {
//     @property(ButtonPlus)
//     about_up: ButtonPlus = null;
//     close: ButtonPlus = null;
// 
//     abount:cc.Sprite = null;
//  
//     onLoad () {
//         //let v = this.getComponent("abount");
// 
//         let btnNode = this.node.getChildByName("close");
//         this.close = btnNode.getComponent(ButtonPlus);
// 
// 
//         let  sNode = this.node.getChildByName("abount");
//         this.abount = sNode.getComponent(cc.Sprite);
// 
// 
//      }
// 
// }
