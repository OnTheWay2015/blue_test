import { _decorator, Component } from 'cc';
const {ccclass, property} = _decorator;

import ButtonPlus from "./../Common/Components/ButtonPlus"

@ccclass('UICaptureAuto')
export default class UICapture_Auto extends Component {
	@property(ButtonPlus)
	Back: ButtonPlus = null;
	@property(ButtonPlus)
	Capture: ButtonPlus = null;
	@property(ButtonPlus)
	Pen: ButtonPlus = null;
	@property(ButtonPlus)
	Reaser: ButtonPlus = null;
 
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import ButtonPlus from "./../Common/Components/ButtonPlus"
// 
// const {ccclass, property} = cc._decorator;
// @ccclass
// export default class UICapture_Auto extends cc.Component {
// 	@property(ButtonPlus)
// 	Back: ButtonPlus = null;
// 	@property(ButtonPlus)
// 	Capture: ButtonPlus = null;
// 	@property(ButtonPlus)
// 	Pen: ButtonPlus = null;
// 	@property(ButtonPlus)
// 	Reaser: ButtonPlus = null;
//  
// }
