import { _decorator, Component } from 'cc';
const {ccclass, property} = _decorator;

import ButtonPlus from "./../Common/Components/ButtonPlus"

@ccclass('UILevelAuto')
export default class UILevel_Auto extends Component {
	@property(ButtonPlus)
	Setting: ButtonPlus = null;
	@property(ButtonPlus)
	Skills: ButtonPlus = null;
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
// export default class UILevel_Auto extends cc.Component {
// 	@property(ButtonPlus)
// 	Setting: ButtonPlus = null;
// 	@property(ButtonPlus)
// 	Skills: ButtonPlus = null;
// 	@property(ButtonPlus)
// 	Back: ButtonPlus = null;
//  
// }
