import { _decorator, Component, Label } from 'cc';
const {ccclass, property} = _decorator;

import ButtonPlus from "./../Common/Components/ButtonPlus"

@ccclass('UIMobxAuto')
export default class UIMobx_Auto extends Component {
	@property(ButtonPlus)
	Close: ButtonPlus = null;
	@property(Label)
	@property(ButtonPlus)
	Btn1: ButtonPlus = null;
	@property(Label)
	@property(ButtonPlus)
	Btn2: ButtonPlus = null;
	@property(Label)
	@property(Label)
	@property(ButtonPlus)
	Btn3: ButtonPlus = null;
	@property(ButtonPlus)
	Btn4: ButtonPlus = null;
 
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import ButtonPlus from "./../Common/Components/ButtonPlus"
// 
// const {ccclass, property} = cc._decorator;
// @ccclass
// export default class UIMobx_Auto extends cc.Component {
// 	@property(ButtonPlus)
// 	Close: ButtonPlus = null;
// 	@property(cc.Label)
// 	Txt1: cc.Label = null;
// 	@property(ButtonPlus)
// 	Btn1: ButtonPlus = null;
// 	@property(cc.Label)
// 	Txt2: cc.Label = null;
// 	@property(ButtonPlus)
// 	Btn2: ButtonPlus = null;
// 	@property(cc.Label)
// 	Txt3: cc.Label = null;
// 	@property(cc.Label)
// 	Txt4: cc.Label = null;
// 	@property(ButtonPlus)
// 	Btn3: ButtonPlus = null;
// 	@property(ButtonPlus)
// 	Btn4: ButtonPlus = null;
// 	@property(cc.Label)
// 	Txt5: cc.Label = null;
//  
// }
