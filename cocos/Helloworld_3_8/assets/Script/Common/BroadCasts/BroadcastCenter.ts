import { _decorator } from 'cc';
import { Broadcast } from "../Utils/BroadCast";

export class BuildingData {
    id: number;
    state: string;
}

export class BroadcastCenter {
    static buildingState = new Broadcast<BuildingData>();    
}

BroadcastCenter.buildingState.on((data) => {
    console.log(data.id, data.state);
}, this);
BroadcastCenter.buildingState.dispatch({id: 1, state: "suss"});


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import { Broadcast } from "../Utils/BroadCast";
// 
// export class BuildingData {
//     id: number;
//     state: string;
// }
// 
// export class BroadcastCenter {
//     static buildingState = new Broadcast<BuildingData>();    
// }
// 
// 
// BroadcastCenter.buildingState.on((data) => {
//     console.log(data.id, data.state);
// }, this);
// 
// 
// BroadcastCenter.buildingState.dispatch({id: 1, state: "success"});
