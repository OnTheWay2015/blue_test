import { _decorator, Component, Node } from 'cc';
const {ccclass, property} = _decorator;

@ccclass('UILoading_Auto')
export default class UILoading_Auto extends Component {
	@property(Node)
 	Left: Node = null;
	@property(Node)
 	Right: Node = null;
 
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// const {ccclass, property} = cc._decorator;
// @ccclass
// export default class UILoading_Auto extends cc.Component {
// 	@property(cc.Node)
// 	Right: cc.Node = null;
// 	@property(cc.Node)
// 	Left: cc.Node = null;
//  
// }
