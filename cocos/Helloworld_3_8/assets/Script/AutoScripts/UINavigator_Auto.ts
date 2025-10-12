import { _decorator, Component, ScrollView } from 'cc';
const {ccclass, property} = _decorator;

@ccclass('UINavigator_Auto')
export default class UINavigator_Auto extends Component {
	@property(ScrollView)
	Scroll: ScrollView = null;
 
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// const {ccclass, property} = cc._decorator;
// @ccclass
// export default class UINavigator_Auto extends cc.Component {
// 	@property(cc.ScrollView)
// 	Scroll: cc.ScrollView = null;
//  
// }
