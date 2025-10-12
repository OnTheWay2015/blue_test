//const ClassMap: {[key: string]: Object} = cc.js.createMap();

const ClassMap= new Map<[key:string], Object>();
//newMap.set('key', value);
//const val = newMap.get('key');

export const RigisterClass = (name: string) => {
    return function (target: any) {
        ClassMap[name] = target;
    }
}

export const GetClassByName = (name: string) => {
    return ClassMap[name];
}

