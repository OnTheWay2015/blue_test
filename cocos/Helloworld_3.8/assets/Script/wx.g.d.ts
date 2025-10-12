// 微信API声明...
declare interface Window {
    wx: {
        onHide: (callback: () => void) => void;
        onShow: (callback: () => void) => void;
        getMenuButtonBoundingClientRect():void; 
        getSystemInfoSync():void; 
    };
}
