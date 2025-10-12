import { _decorator } from 'cc';
export class EnumUtils {
    static getNamesAndValues(e: any) {
        // return this.getNames(e).map(_name => { return { name: _name, value: e[_name] as number }; });
    }
    static getNames(e: any) {
        // return this.getObjectValues(e).filter(v => typeof v === "string") as string[];
    }
    static getValues(e: any) {
        // return this.getObjectValues(e).filter(v => typeof v === "number") as number[];
    }
    private static getObjectValues(e: any): (number | string)[] {
         return Object.keys(e).map(k => e[k]);
    }
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// export class EnumUtils {
//     static getNamesAndValues(e: any) {
//         return this.getNames(e).map(_name => { return { name: _name, value: e[_name] as number }; });
//     }
// 
//     static getNames(e: any) {
//         return this.getObjectValues(e).filter(v => typeof v === "string") as string[];
//     }
// 
//     static getValues(e: any) {
//         return this.getObjectValues(e).filter(v => typeof v === "number") as number[];
//     }
// 
//     private static getObjectValues(e: any): (number | string)[] {
//         return Object.keys(e).map(k => e[k]);
//     }
// }
