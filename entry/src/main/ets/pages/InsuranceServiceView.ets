@Entry
@Component
struct InsuranceServiceView {
  @State currentStep: number = 1;
  @State insuranceType: string = 'compulsory';
  @State paymentMethod: string = 'alipay';
  @State plateNumber: string = '';
  @State carBrand: string = '';
  @State carModel: string = '';

  build() {
    Column() {
      // 顶部标题
      Text('车险办理')
        .fontSize(20)
        .fontWeight(FontWeight.Bold)
        .margin({ top: 20 });

      // 步骤显示
      Text(`当前步骤：${this.currentStep}/3`)
        .fontSize(16)
        .margin({ top: 20 });

      // 内容区域
      Column() {
        if (this.currentStep === 1) {
          this.step1Content();
        } else if (this.currentStep === 2) {
          this.step2Content();
        } else if (this.currentStep === 3) {
          this.step3Content();
        }
      }
      .width('100%')
      .padding(15)
      .margin({ top: 20 })
      .backgroundColor('#FFFFFF')
      .borderRadius(8)

      // 底部按钮
      Row() {
        if (this.currentStep > 1) {
          Button('上一步')
            .width('45%')
            .onClick(() => this.currentStep--)
            .backgroundColor('#FFFFFF')
            .fontColor('#1677ff')
            .border({ width: 1, color: '#1677ff' })
            .margin({ right: 10 });
        }
        Button(this.currentStep === 3 ? '完成' : '下一步')
          .width(this.currentStep === 1 ? '100%' : '45%')
          .onClick(() => {
            if (this.currentStep < 3) {
              this.currentStep++;
            } else {
              console.log('车险办理完成', {
                insuranceType: this.insuranceType,
                plateNumber: this.plateNumber,
                carBrand: this.carBrand,
                carModel: this.carModel,
                paymentMethod: this.paymentMethod
              });
            }
          })
          .backgroundColor('#1677ff')
          .fontColor('#FFFFFF');
      }
      .width('100%')
      .height(60)
      .padding(15)
      .backgroundColor('#FFFFFF')
      .position({ bottom: 0 })
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#F5F5F5');
  }

  // 保险类型选择
  @Builder step1Content() {
    Column() {
      Text('1. 选择保险类型')
        .fontSize(16)
        .fontWeight(FontWeight.Bold)
        .margin({ bottom: 10 });

      Column() {
        Row() {
          Radio({ value: 'compulsory', group: 'insuranceType' })
            .checked(this.insuranceType === 'compulsory')
            .onChange((isChecked: boolean) => {
              if (isChecked) this.insuranceType = 'compulsory';
            })
          Text('交强险')
            .fontSize(14)
            .margin({ left: 10 });
        }
        .margin({ bottom: 10 });

        Row() {
          Radio({ value: 'thirdParty', group: 'insuranceType' })
            .checked(this.insuranceType === 'thirdParty')
            .onChange((isChecked: boolean) => {
              if (isChecked) this.insuranceType = 'thirdParty';
            })
          Text('第三者责任险')
            .fontSize(14)
            .margin({ left: 10 });
        }
        .margin({ bottom: 10 });

        Row() {
          Radio({ value: 'vehicleDamage', group: 'insuranceType' })
            .checked(this.insuranceType === 'vehicleDamage')
            .onChange((isChecked: boolean) => {
              if (isChecked) this.insuranceType = 'vehicleDamage';
            })
          Text('车损险')
            .fontSize(14)
            .margin({ left: 10 });
        }
        .margin({ bottom: 10 });

        Row() {
          Radio({ value: 'allRisk', group: 'insuranceType' })
            .checked(this.insuranceType === 'allRisk')
            .onChange((isChecked: boolean) => {
              if (isChecked) this.insuranceType = 'allRisk';
            })
          Text('全险')
            .fontSize(14)
            .margin({ left: 10 });
        }
      }
      .margin({ top: 10 })
    }
  }

  //车辆信息
  @Builder step2Content() {
    Column() {
      Text('2. 填写车辆信息')
        .fontSize(16)
        .fontWeight(FontWeight.Bold)
        .margin({ bottom: 20 });

      TextInput({ placeholder: '请输入车牌号' })
        .width('100%')
        .margin({ bottom: 15 })
        .onChange((value: string) => {
          this.plateNumber = value;
        });

      TextInput({ placeholder: '请输入车辆品牌' })
        .width('100%')
        .margin({ bottom: 15 })
        .onChange((value: string) => {
          this.carBrand = value;
        });

      TextInput({ placeholder: '请输入车辆型号' })
        .width('100%')
        .onChange((value: string) => {
          this.carModel = value;
        });
    }
  }

  // 支付
  @Builder step3Content() {
    Column() {
      Text('3. 支付保费')
        .fontSize(16)
        .fontWeight(FontWeight.Bold)
        .margin({ bottom: 20 });

      Row() {
        Text('应付金额:')
          .fontSize(14);
        Text('¥ 2560.00')
          .fontSize(18)
          .fontColor('#FF0000')
          .margin({ left: 10 });
      }
      .margin({ bottom: 20 });

      Text('选择支付方式:')
        .fontSize(14)
        .margin({ bottom: 10 });

      Column() {
        Row() {
          Radio({ value: 'alipay', group: 'paymentMethod' })
            .checked(this.paymentMethod === 'alipay')
            .onChange((isChecked: boolean) => {
              if (isChecked) this.paymentMethod = 'alipay';
            })
          Text('支付宝')
            .fontSize(14)
            .margin({ left: 10 });
        }
        .margin({ bottom: 10 });

        Row() {
          Radio({ value: 'wechat', group: 'paymentMethod' })
            .checked(this.paymentMethod === 'wechat')
            .onChange((isChecked: boolean) => {
              if (isChecked) this.paymentMethod = 'wechat';
            })
          Text('微信支付')
            .fontSize(14)
            .margin({ left: 10 });
        }
        .margin({ bottom: 10 });

        Row() {
          Radio({ value: 'bank', group: 'paymentMethod' })
            .checked(this.paymentMethod === 'bank')
            .onChange((isChecked: boolean) => {
              if (isChecked) this.paymentMethod = 'bank';
            })
          Text('银行卡支付')
            .fontSize(14)
            .margin({ left: 10 });
        }
      }
    }
  }
}