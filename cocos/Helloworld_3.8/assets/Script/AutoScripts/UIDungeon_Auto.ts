import { _decorator, Component, Node } from 'cc';
const {ccclass, property} = _decorator;

import ButtonPlus from "./../Common/Components/ButtonPlus"

@ccclass('UIDungeonAuto')
export default class UIDungeon_Auto extends Component {
	@property(Node)
	@property(ButtonPlus)
	Back: ButtonPlus = null;
 
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import ButtonPlus from "./../Common/Components/ButtonPlus"
// 
// const {ccclass, property} = cc._decorator;
// @ccclass
// export default class UIDungeon_Auto extends cc.Component {
// 	@property(cc.Node)
// 	Items: cc.Node = null;
// 	@property(ButtonPlus)
// 	Back: ButtonPlus = null;
//  
// }
