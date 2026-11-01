import promptAction from '@ohos.promptAction';

@Entry
@Component
export struct TemporaryLicenseView {
  @State vehicleType: string = ''
  @State engineNumber: string = ''
  @State frameNumber: string = ''

  @State showVehicleTypeHint: boolean = true
  @State showEngineNumberHint: boolean = true
  @State showFrameNumberHint: boolean = true

  build() {
    Column() {
      Text('临牌申领')
        .fontSize(20)
        .fontWeight(500)
        .margin(16)

      Column() {
        Text('车辆类型')
          .fontSize(14)
          .fontColor('#666')
          .margin({ top: 12, bottom: 4 })

        if (this.showVehicleTypeHint && !this.vehicleType) {
          Text('请选择车辆类型（小型汽车/大型汽车等）')
            .fontSize(14)
            .fontColor('#999')
            .backgroundColor('#F5F5F5')
            .padding(12)
            .border({ width: 1, color: '#eee' })
            .borderRadius(8)
            .onClick(() => this.showVehicleTypeHint = false)
        } else {
          TextInput()
            .fontSize(14)
            .border({ width: 1, color: '#eee' })
            .borderRadius(8)
            .padding(12)
            .onChange((value) => this.vehicleType = value)
            .onBlur(() => {
              if (!this.vehicleType) this.showVehicleTypeHint = true
            })
        }
      }
      .margin({ left: 16, right: 16 })

      Column() {
        Text('发动机号')
          .fontSize(14)
          .fontColor('#666')
          .margin({ top: 12, bottom: 4 })

        if (this.showEngineNumberHint && !this.engineNumber) {
          Text('请输入发动机后6位')
            .fontSize(14)
            .fontColor('#999')
            .backgroundColor('#F5F5F5')
            .padding(12)
            .border({ width: 1, color: '#eee' })
            .borderRadius(8)
            .onClick(() => this.showEngineNumberHint = false)
        } else {
          TextInput()
            .fontSize(14)
            .border({ width: 1, color: '#eee' })
            .borderRadius(8)
            .padding(12)
            .onChange((value) => this.engineNumber = value)
            .onBlur(() => {
              if (!this.engineNumber) this.showEngineNumberHint = true
            })
        }
      }
      .margin({ left: 16, right: 16 })

      Column() {
        Text('车架号')
          .fontSize(14)
          .fontColor('#666')
          .margin({ top: 12, bottom: 4 })

        if (this.showFrameNumberHint && !this.frameNumber) {
          Text('请输入车架号后6位')
            .fontSize(14)
            .fontColor('#999')
            .backgroundColor('#F5F5F5')
            .padding(12)
            .border({ width: 1, color: '#eee' })
            .borderRadius(8)
            .onClick(() => this.showFrameNumberHint = false)
        } else {
          TextInput()
            .fontSize(14)
            .border({ width: 1, color: '#eee' })
            .borderRadius(8)
            .padding(12)
            .onChange((value) => this.frameNumber = value)
            .onBlur(() => {
              if (!this.frameNumber) this.showFrameNumberHint = true
            })
        }
      }
      .margin({ left: 16, right: 16 })

      Column() {
        Text('上传车辆照片')
          .fontSize(14)
          .fontColor('#666')
          .margin({ top: 24, left: 16, bottom: 8 })

        Row() {
          Image('icon_upload.png')
            .width(80)
            .height(80)
            .border({ width: 1, color: '#eee' })
            .borderRadius(8)
            .margin(16)
            .onClick(() => promptAction.showToast({ message: '图片上传功能待实现' }))

          Image('icon_upload.png')
            .width(80)
            .height(80)
            .border({ width: 1, color: '#eee' })
            .borderRadius(8)
            .margin(16)
            .onClick(() => promptAction.showToast({ message: '图片上传功能待实现' }))
        }
      }

      Button('提交申请')
        .fontSize(16)
        .fontColor('#fff')
        .backgroundColor('#1890ff')
        .height(48)
        .margin({ top: 32, left: 16, right: 16 })
        .borderRadius(24)
        .onClick(() => {
          if (!this.vehicleType || !this.engineNumber || !this.frameNumber) {
            promptAction.showToast({ message: '请填写完整信息' })
            return
          }
          promptAction.showToast({ message: '提交成功，审核中...' })
        })
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#f5f5f5')
  }
}