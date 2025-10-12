import { _decorator, Component, Label } from 'cc';
const {ccclass, property} = _decorator;

@ccclass('UITipsAuto')
export default class UITips_Auto extends Component {
	@property(Label)
 	Tips: Label = null;
 
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// const {ccclass, property} = cc._decorator;
// @ccclass
// export default class UITips_Auto extends cc.Component {
// 	@property(cc.Label)
// 	Tips: cc.Label = null;
//  
// }
