//import incomeconfig  from "./testInCome.json"
//import incomeconfig  from "./testInCome_stop.json"
import * as FS from 'fs';

let STATIC_INCOME = 9.8;
let AUTO_ADD_MEMBER= false;
//let STATIC_INCOME = 10000;
let VIP_RATE = [
    0,
    5,
    10,
    15,
    20,
    25,
    30
];

let VIP_MEMber= [
    0,
    1,
    5,
    15,
    50,
    100,
    150
];


class player {
    public ID!:number; //编号
    public parentID!:number; //上级编号
    public sub_IDs:number[]=[]; //下级
    public vip!:number; //vip等级

    public incomeExFrom:CalcSt[]=[];//子级额外收益
    public incomeEx!:number; //vip 增加收益
    public income!:number; //固定收益
    public outExFrom:CalcSt[]=[];//被抽取到上级的收益
    public visit:boolean=false;//是否计算过
}

class CalcSt{
    public ID_base:number=0; // 基数来源
    public ID_form:number=0; // 抽取自
    public ID_to:number=0; // 上交到
    public value_base:number=0; //计算基数
    public value:number=0; //收益
    public valuetp:string = ""; //收益计算类型
    public vip_distance:number=0; //级差
    public rate:number=0; //系数
}


export class testIncome {
    private test:string = `[
        {
            "ID":1,
            "parentID":0,
            "vip":1
        }
        ,{
            "ID":2,
            "parentID":1,
            "vip":2
        }

    ]`;
    private outstr: string = "";
    start() {
        let incomeconfig = this.loadConfig();
        STATIC_INCOME = incomeconfig.static_income ? incomeconfig.static_income : 9.8;
        AUTO_ADD_MEMBER= incomeconfig.auto_add_member? incomeconfig.auto_add_member:false; 

        this.calc(incomeconfig, this.outstr)
        //console.log(this.outstr);
        FS.writeFileSync("www.txt",this.outstr);
    }
    //start() {
    //    let cav = this.node.getComponent(Canvas);
    //    let btn = cav.getComponentInChildren(Button);
    //    let eb = cav.getComponentInChildren(EditBox);
    //    let rt = cav.getComponentInChildren(RichText);
    //    btn.node.on(Node.EventType.TOUCH_END, (e: EventTouch) => {
    //        //this.calc(this.test,this.outstr)
    //        this.calc(eb.string,this.outstr)
    //        console.log(this.outstr);
    //        rt.string = this.outstr;
    //    }, btn);
    //}

    loadConfig() {
        const rawData = FS.readFileSync('./configs/testInCome.json', 'utf8');
        const config = JSON.parse(rawData);
        return config;
    }

    update(deltaTime: number) {
        
    }

    private calc(config: any, outstr: string) {
        //let i:any = JSON.parse(inputStr);
        //this.addlog(outstr,JSON.stringify(i));
        let plys: Map<number, player> = this.formPlys(config.players, outstr);
        this.calcPlys(plys, outstr);
        //this.logOutPlysLeveL(plys);
        this.logOutPlys(plys, outstr);
    }

    //private logOutPlysLeveL(plys: Map<number,player>) {
    //}

    //private logOutPlyLevel(ply:player,plys: Map<number,player>){
    //}

    private addlog(_:string,addstr:string){
        addstr = this.formstr(addstr);
        this.outstr += this.outstr=="" ? "" :  "\n";
        this.outstr += addstr;
    }
    private logOutPlys(plys: Map<number,player>,outstr:string) {
        for (const [key, ply] of plys) {
            if (ply.parentID){
                continue;
            }
            this.addlog(outstr,`TOP ID[${ply.ID}]=========================================`);
            this.logOutPly(ply,plys,outstr);
            this.addlog(outstr,"=========================================");
            this.addlog(outstr,"\n")
            this.addlog(outstr,"\n")
        }
    }

    public formstr(ss: string):string {
        ss = ss.trim();
        ss = ss.replace(/\n/g, "");
        ss = ss.replace(/    /g, "");
        ss = ss.replace(/\t/g, " ");
        return ss;
    }
    private logOutPly(ply:player,plys: Map<number,player>,outstr:string){
        let invalue  = 0;
        let outvalue = 0;

        ply.outExFrom.forEach(element => {
            outvalue += element.value; 
            
//            let ss = `被谁扣了奖励[${element.ID_to}]
//                奖励的基数来源[${element.ID_base}]
//                额外奖励[${element.value}]
//                计算额外奖励的基数[${element.value_base}]
//                是否有级差[${element.valuetp}]
//                vip额外奖励系数[${element.rate}]
//                vip级差[${element.vip_distance}]`
//
            this.addlog(outstr,`   
                  被谁扣了奖励[${element.ID_to}],
                奖励的基数来源[${element.ID_base}],
                额外奖励[${element.value}],
                计算额外奖励的基数[${element.value_base}],
                是否有级差[${element.valuetp}],
                vip额外奖励系数[${element.rate}],
                vip级差[${element.vip_distance}]
            `);
        });

        ply.incomeExFrom.forEach(element => {
            invalue += element.value; 
            this.addlog(outstr,`   
                  奖励的来源[${element.ID_form}], 
                奖励的基数来源[${element.ID_base}],
                额外奖励[${element.value}],
                计算额外奖励的基数[${element.value_base}],
                是否有级差[${element.valuetp}],
                vip额外奖励系数[${element.rate}],
                vip级差[${element.vip_distance}]
            `);
        });
       
        let v = this.formNumber(invalue-outvalue);
        //this.addlog(outstr,`**ply[${ply.ID}], vip[${ply.vip}], total incomeEx[${invalue-outvalue}], outEx[${outvalue}],`);
        this.addlog(outstr,`**用户[${ply.ID}], vip[${ply.vip}], 总额外奖励[${v}], 上级扣除[${outvalue}]`);

        this.addlog(outstr,"------------------");

        //sub
        ply.sub_IDs.forEach(id=> {
            let p = plys.get(id);
            if (!p) {
                console.error("id error!");
                return;

            }
            this.logOutPly(p,plys,outstr);
        });
    }


    private formPlys(objs:any[],outstr:string):Map<number,player>{
        let plys = new Map<number, player>();
        objs.forEach(e => {
            let p = new player();
            Object.assign(p, e);
            plys.set(p.ID,p); 
        })

        //init subs
        for (const [id,ply] of plys){
            if (ply.parentID <= 0){
                continue;
            }
            let pp:player|undefined = plys.get(ply.parentID);
            if (!pp){
                console.error("id error!");
                continue;
            }
            pp.sub_IDs.push(ply.ID);
        }

        if (!AUTO_ADD_MEMBER){
            return plys;
        }
        //check vip
        let addplys:player[]= []; 
        for (const [id,ply] of plys){
            let memcnt:number = VIP_MEMber[ply.vip];
            let n:number =memcnt - ply.sub_IDs.length;
            if (n<=0){
                continue;
            }
            for (let i = 0; i < n; i++) {
                let p = new player();
                p.vip = 0;
                p.ID = ply.ID * 1000000 + i +1;
                p.parentID = ply.ID
                addplys.push(p);
            }
        }
        addplys.forEach(p=>{
            plys.set(p.ID,p);
        });

        return plys;
    }

    private calcPlys(plys:Map<number,player>,outstr:string){
        for (const [id,ply] of plys){
           this.calcPly(ply,plys,outstr); 
        }
    }
    private makeCalcSt(baseid:number,id:number,isout:boolean=false):CalcSt{
        let c =new CalcSt();
        c.ID_base = baseid;
        isout? c.ID_to = id : c.ID_form = id;
        return c;
    }
    private calcPly(ply:player,plys:Map<number,player>,outstr:string){
        if (ply.visit){
            console.error(`ply[${ply.ID}] visit`);
            return;
        }
        ply.visit = true;
        this.calcInCome(ply);
        //if (ply.parentID > 0 ) {
        if (ply.sub_IDs.length <= 0 ) {
            let next:player|undefined = plys.get(ply.parentID);
            let lmvip = this.calcLineMaxVip(ply,plys);
            this.calcLineInCome(
                ply,
                ply,
                next,
                0,//ply.vip,
                false,
                lmvip,
                null,
                plys,
            outstr);
        } 
    }

    private calcInCome(ply:player){
        let vipRate =[];
        ply.income = STATIC_INCOME;//固定收益
        ply.incomeEx =ply.income *(VIP_RATE[ply.vip])/100; //vip 增加收益
    }

    private calcLineMaxVip(ply:player,plys:Map<number,player>):number{
        //let vip = ply.vip; 
        let vip = 0; //不从自己算起 
        while (ply.parentID > 0 ){
            ply = plys.get(ply.parentID)!;
            if (vip < ply.vip){
                vip = ply.vip;
            }
        }

        return vip;
    }

    private formNumber(v:number):number{
        return Math.floor(v*1000000)/1000000;
    }

    //每推广一个用户，就应该多一条该用户影响的太阳线
    //推广线收益
    private calcLineInCome(baseply:player,prePly:player,ply:player|undefined,maxvip:number, preExtracted:boolean, lineMaxVip:number, preInCome:CalcSt|null, plys:Map<number,player>,outstr:string,stop:boolean=false){
        if (!ply){
            console.error("ply error!");
            return;
        }
        let base:number  = (baseply.income + baseply.incomeEx);
        let plyVip = ply.vip;
        let inSt = this.makeCalcSt(baseply.ID,prePly.ID); 
        inSt.ID_base = baseply.ID;
        if (plyVip > maxvip){
            preExtracted = false;
            inSt.vip_distance = plyVip - maxvip;
            let rate = (VIP_RATE[plyVip] - VIP_RATE[maxvip]);
            inSt.rate = rate;
            inSt.value_base = this.formNumber( base );
            inSt.value= this.formNumber( base * inSt.rate/100); //vip 增加收益

            maxvip = plyVip;
            inSt.valuetp = "正常";
            ply.incomeExFrom.push(inSt);
        } else {
            //vip 平级或级差增加收益, 要从子级里扣
            if (!preExtracted) { //扣过的，就不要无限扣
                inSt.rate = 10;
                inSt.valuetp = "级差";
                if (preInCome) {
                    //inSt.value=  preInCome.value* 10/100 ;
                    inSt.value = this.formNumber(preInCome.value * 10 / 100);

                    let outSt = this.makeCalcSt(baseply.ID, ply.ID, true);
                    outSt.rate = inSt.rate;
                    outSt.value = inSt.value;
                    outSt.value_base = preInCome.value;
                    outSt.valuetp = inSt.valuetp;
                    //outSt.vip_distance = inSt.vip_distance;
                    prePly.outExFrom.push(outSt);

                } else {
                    //进入计算时，太阳线最底的玩家vip等级要高于他的父级vip等级时 会触发
                    inSt.value = this.formNumber(base * 10 / 100);
                }
                ply.incomeExFrom.push(inSt);
            }
            preExtracted = true;
        }
        
        if (ply.parentID > 0 && !stop ){
            this.calcLineInCome(baseply,ply,plys.get(ply.parentID),maxvip,preExtracted,lineMaxVip, inSt, plys,outstr, maxvip >= lineMaxVip );
        }
    }

}




