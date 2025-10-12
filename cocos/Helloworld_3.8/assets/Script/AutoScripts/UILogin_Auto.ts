import { _decorator, Component, Node, Label, Button } from 'cc';
const {ccclass, property} = _decorator;

//import ButtonPlus from "./../Common/Components/ButtonPlus"

@ccclass('UILoginAuto')
export default class UILogin_Auto extends Component {
	@property(Button)
	btn: Button= null;
 
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import ButtonPlus from "./../Common/Components/ButtonPlus"
// 
// const {ccclass, property} = cc._decorator;
// @ccclass
// export default class UILogin_Auto extends cc.Component {
// 	@property(cc.Node)
// 	Login: cc.Node = null;
// 	@property(ButtonPlus)
// 	btn: ButtonPlus = null;
// 	@property(cc.Label)
// 	Name: cc.Label = null;
//  
// }
