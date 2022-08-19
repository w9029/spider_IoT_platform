// pages/tabbar/tabbar.js

var mac = "";
var relays = new Array();
var machines = new Array();
var recv_control = 3;
var s_head = ["null", "null", "null", "null", "null", "null", "null", "null", "null"];
var s_text = ["null", "null", "null", "null", "null", "null", "null", "null", "null"];

Page({
  data: {

    index: 0,
    /**
     * 走马灯
     */
    temp: "",
    switchhide: true, //继电器开关隐藏
    sw_state: [false, false, false, false, false, false, false, false],
    sw_name: ["引脚0", "引脚1", "引脚2", "引脚3", "引脚4", "引脚5", "引脚6", "引脚7"],
    sensor_head: ["null", "null", "null", "null", "null", "null", "null", "null", "null"],
    sensor_text: ["null", "null", "null", "null", "null", "null", "null", "null", "null"],
    sensor_num: 0,
    text: '欢迎使用 智慧农业产业数据管理系统！',
    marqueePace: 2.5, //滚动速度
    marqueeDistance: 3, //初始滚动距离
    marqueeDistance2: 0,
    marquee2copy_status: false,
    marquee2_margin: 60,
    size: 14,
    orientation: 'left', //滚动方向
    interval: 30  // 时间间隔
  },
  changeTabbar(e) {
    this.setData({ index: e.currentTarget.dataset.id })
  },
  bindMAC: function (e) {
    this.setData({
      temp: e.detail.value,
    });
  },
  bindbuttom: function (e) {
    mac = this.data.temp;
    let vm = this;
    //建立连接
    wx.connectSocket({
      url: "ws://192.168.100.94:8050/webSocket",
    });
    //连接成功
    wx.onSocketOpen(function () {
      wx.sendSocketMessage({
        data: mac
      });
      wx.showToast({
        title: 'Link start！',
        icon: 'success',
        duration: 2000
      })
    });

    wx.onSocketMessage(function (data) {
      if (recv_control >= 3) {
        try {
          var objData = JSON.parse(data.data);

          console.log(objData["sensor"][0]["name"]);

          //取出继电器设备
          var relay_position;
          for (var i = 0; i < objData["relay"][0]["valueList"].length; i++) {
            relay_position = parseInt(objData["relay"][0]["valueList"][i]["position"]);
            relays[relay_position] = objData["relay"][0]["valueList"][i]["message"];
            machines[relay_position] = objData["relay"][0]["valueList"][i]["name"]
          }
          console.log(relays);
          console.log(machines);

          //取出传感器信息
          console.log("*********" + objData["sensor"].length);
          var sensor_len = objData["sensor"].length;
          for (var i = 0; i < objData["sensor"].length; i++) {
            s_head[i] = objData["sensor"][i]["name"];
            s_text[i] = objData["sensor"][i]["message"];
            s_text[i] = s_text[i].split(',').join('\n');
          }
        } catch (err) {
          console.log(data.data)
        }
      }
      recv_control = recv_control + 1;
      vm.setData({
        switchhide: false,
        sw_state: relays,
        sw_name: machines,
        sensor_head: s_head,
        sensor_text: s_text,
        sensor_num: sensor_len
      })
    });

    wx.onSocketError(function () {
      console.log('websocket 连接失败!');
      wx.showToast({
        title: 'Link failed！',
        icon: 'failed',
        duration: 2000
      })
    });

    wx.onSocketClose(function () {
      console.log('websocket 连接断开!')
    })
  },
  //跑马灯
  unbindbuttom: function () {
    let vm = this;
    wx.closeSocket();
    wx.showToast({
      title: 'Link off！',
      icon: 'success',
      duration: 2000
    });
    vm.setData({
      sw_state: [false, false, false, false, false, false, false, false],
      sw_name: ["引脚0", "引脚1", "引脚2", "引脚3", "引脚4", "引脚5", "引脚6", "引脚7"],
      sensor_num: 0
    })
  },
  onShow: function () {
    // 页面显示
    let vm = this;
    let length = vm.data.text.length * vm.data.size; //文字长度
    let windowWidth = wx.getSystemInfoSync().windowWidth; // 屏幕宽度
    vm.setData({
      length: length,
      windowWidth: windowWidth,
      marquee2_margin: length < windowWidth ? windowWidth - length : vm.data.marquee2_margin //当文字长度小于屏幕长度时，需要增加补白
    });
    vm.run(); // 水平一行字滚动完了再按照原来的方向滚动
  },
  run: function () {
    let vm = this;
    let interval = setInterval(function () {
      if (-vm.data.marqueeDistance < vm.data.length) {
        vm.setData({
          marqueeDistance: vm.data.marqueeDistance - vm.data.marqueePace,
        });
      } else {
        clearInterval(interval);
        vm.setData({
          marqueeDistance: vm.data.windowWidth
        });
        vm.run();
      }
    }, vm.data.interval);
  },
  //继电器遥控器
  changeSwitch1: function () {
    let vm = this;
    vm.relayopt(0, vm.data.sw_state[0])
  },
  changeSwitch2: function () {
    let vm = this;
    vm.relayopt(1, vm.data.sw_state[1])
  },
  changeSwitch3: function () {
    let vm = this;
    vm.relayopt(2, vm.data.sw_state[2])
  },
  changeSwitch4: function () {
    let vm = this;
    vm.relayopt(3, vm.data.sw_state[3])
  },
  changeSwitch5: function () {
    let vm = this;
    vm.relayopt(4, vm.data.sw_state[4])
  },
  changeSwitch6: function () {
    let vm = this;
    vm.relayopt(5, vm.data.sw_state[5])
  },
  changeSwitch7: function () {
    let vm = this;
    vm.relayopt(6, vm.data.sw_state[6])
  },
  changeSwitch8: function () {
    let vm = this;
    vm.relayopt(7, vm.data.sw_state[7])
  },

  relayopt: function (num, now_state) {
    let vm = this;
    console.log(num);
    console.log(now_state);
    wx.request({
      url: "http://192.168.100.94:8050/user/down/control",
      method: "POST",
      data: {
        target: "relay",
        deviceId: mac,
        operation: now_state == 0 ? "On1" : "Off1",
        addr: "254",
        content: num
      },
      header: {
        "Content-Type": "application/x-www-form-urlencoded"
      },
      success: function (res) {
        console.log(res.data);
        vm.data.sw_state[num] = !vm.data.sw_state[num];
        recv_control = 0;
        wx.showToast({
          title: '操作成功！',
          icon: 'success',
          duration: 2000
        })
      },
    })
  }

})