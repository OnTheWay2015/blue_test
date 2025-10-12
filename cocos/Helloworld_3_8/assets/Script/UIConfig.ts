import { _decorator, error, game, Game,js } from 'cc';
import { SysDefine } from './UIFrame/config/SysDefine';
import { IFormConfig } from './UIFrame/Struct';

 

 
export class UIConfigManager {
    private UIConfigs: any = [
        {
            uiname: "UIFixed",
            prefabUrl: "Forms/Fixed/UIFunction",
            type: "UIFixed"
        }
        , {
            uiname: "UISound",
            prefabUrl: "Forms/Fixed/UISound",
            type: "UIFixed"
        }
        , {
            uiname: "UIAbout",
            prefabUrl: "Forms/Screen/UIAbout",
            type: "UIScreen"
        }
        , {
            uiname: "capture",
            prefabUrl: "Forms/Screen/UICapture",
            type: "UIScreen"
        }
        , {
            uiname: "dungeon",
            prefabUrl: "Forms/Screen/UIDungeon",
            type: "UIScreen"
        }
        , {
            uiname: "UIECSView",
            prefabUrl: "Forms/Screen/UIECSView",
            type: "UIScreen"
        }
        , {
            uiname: "UIGame",
            prefabUrl: "Forms/Screen/UIGame",
            type: "UIScreen"
        }
        , {
            uiname: "project",
            prefabUrl: "Forms/Screen/UIHome",
            type: "UIScreen"
        }
        , {
            uiname: "light",
            prefabUrl: "Forms/Screen/UILight",
            type: "UIScreen"
        }
        , {
            uiname: "UIMap",
            prefabUrl: "Forms/Screen/UIMap",
            type: "UIScreen"
        }
        , {
            uiname: "meshTexture",
            prefabUrl: "Forms/Screen/UIMeshTexture",
            type: "UIScreen"
        }
        , {
            uiname: "UINavigator",
            prefabUrl: "Forms/Screen/UINavigator",
            type: "UIScreen"
        }
        , {
            uiname: "scrollTexture",
            prefabUrl: "Forms/Screen/UIScrollTexture",
            type: "UIScreen"
        }
        , {
            uiname: "splitTexture",
            prefabUrl: "Forms/Screen/UISplitTexture",
            type: "UIScreen"
        }
        , {
            uiname: "UILoading",
            prefabUrl: "Forms/Tips/UILoading",
            type: "UITips"
        }
        , {
            uiname: "toast",
            prefabUrl: "Forms/Toast/UIToast1",
            type: "UIToast"
        }
        , {
            uiname: "mobx",
            prefabUrl: "Forms/Windows/UIMobx",
            type: "UIWindow"
        }
        , {
            uiname: "UIPop",
            prefabUrl: "Forms/Windows/UIPop",
            type: "UIWindow"
        }
        , {
            uiname: "UIScrollPlus",
            prefabUrl: "Forms/Windows/UIScrollPlus",
            type: "UIWindow"
        }
        , {
            uiname: "UISetting",
            prefabUrl: "Forms/Windows/UISetting",
            type: "UIWindow"
        }
        , {
            uiname: "UISkills",
            prefabUrl: "Forms/Windows/UISkills",
            type: "UIWindow"
        }
        , {
            uiname: "UITips",
            prefabUrl: "Forms/Windows/UITips",
            type: "UIWindow"
        }
    ]


    private configs: { [key: string]: IFormConfig } = js.createMap();

    private static instance: UIConfigManager = null;                                                 // 单例
    public static getInstance(): UIConfigManager {
        if (this.instance == null) {
            this.instance = new UIConfigManager();
        }
        return this.instance;
    }
    public init() {
        let self = this;
        self.UIConfigs.forEach(info => {
            let key = info.uiname;
            let constourt:any = js.getClassByName(key);
            if (!constourt) {
                let urls = info.prefabUrl.split('/') as string[];
                if (!urls || urls.length <= 0) return;
                let name = urls[urls.length - 1];
                constourt = js.getClassByName(name);
                if (!constourt) {
                    return;
                }
                //key = name;
                //info.uiname = name;
            }

            constourt["UIConfig"] = info;
            self.configs[key] = info;
            if (key == "UILoading") {
                SysDefine.defaultLoadingForm = info as IFormConfig;
            }
        });
    }

    public getFormConfig(uiname: string): IFormConfig {
        let self = this;
        let conf = self.configs[uiname] as IFormConfig;
        if (!conf) {
            error(`窗体[${uiname}]没有对应配置! `);
            return null;
        }
        return conf;
    }
}


game.on(Game.EVENT_GAME_INITED, () => {
    //if(CC_EDITOR) return;
    UIConfigManager.getInstance().init();
});