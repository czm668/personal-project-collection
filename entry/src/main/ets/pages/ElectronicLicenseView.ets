import { router } from '@kit.ArkUI'
interface GeneratedObjectLiteralInterface_1 {
  name: string;
  gender: string;
  nationality: string;
  birthDate: string;
  address: string;
  licenseNumber: string;
  licenseType: string;
  issueDate: string;
  validFrom: string;
  validTo: string;
  licenseImage: Resource;
}

@Entry
@Component
export struct ElectronicLicenseView {
  private licenseInfo: GeneratedObjectLiteralInterface_1 = {
    name: '张三',
    gender: '男',
    nationality: '中国',
    birthDate: '1990-01-01',
    address: '北京市朝阳区XX路XX号',
    licenseNumber: '11010519900101XXXX',
    licenseType: 'C1',
    issueDate: '2015-01-01',
    validFrom: '2015-01-01',
    validTo: '2025-01-01',
    licenseImage: $r('app.media.license')
  };

  build() {
    Column({ space: 15 }) {
      // 头部导航栏
      Row() {
        Button('返回')
          .onClick(() => router.pushUrl({ url: 'pages/ServiceView' }))
          .width(80)
          .height(30)
          .backgroundColor('#007DFF')
          .fontColor('#FFFFFF')
          .borderRadius(5)

        Text('我的电子驾照')
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
          .margin({ left: 80 })
      }
      .width('100%')
      .height(60)
      .backgroundColor('#F5F5F5')
      .padding({ left: 15, right: 15 })

      // 驾照图片
      Image(this.licenseInfo.licenseImage)
        .width('90%')
        .height(200)
        .objectFit(ImageFit.Contain)
        .margin({ top: 15 })
        .borderRadius(10)

      // 个人信息
      Row() {
        Text('姓名')
          .fontSize(16)
          .width(80)

        Text(this.licenseInfo.name)
          .fontSize(16)
          .width('70%')
          .maxLines(1)
      }
      .width('100%')
      .height(40)

      Row() {
        Text('性别')
          .fontSize(16)
          .width(80)

        Text(this.licenseInfo.gender)
          .fontSize(16)
          .width('70%')
          .maxLines(1)
      }
      .width('100%')
      .height(40)

      // 其他信息项
      Row() {
        Text('国籍')
          .fontSize(16)
          .width(80)

        Text(this.licenseInfo.nationality)
          .fontSize(16)
          .width('70%')
          .maxLines(1)
      }
      .width('100%')
      .height(40)

      Row() {
        Text('出生日期')
          .fontSize(16)
          .width(80)

        Text(this.licenseInfo.birthDate)
          .fontSize(16)
          .width('70%')
          .maxLines(1)
      }
      .width('100%')
      .height(40)

      Row() {
        Text('住址')
          .fontSize(16)
          .width(80)

        Text(this.licenseInfo.address)
          .fontSize(16)
          .width('70%')
          .maxLines(1)
      }
      .width('100%')
      .height(40)

      Row() {
        Text('证件号码')
          .fontSize(16)
          .width(80)

        Text(this.licenseInfo.licenseNumber)
          .fontSize(16)
          .width('70%')
          .maxLines(1)
      }
      .width('100%')
      .height(40)

      Row() {
        Text('准驾车型')
          .fontSize(16)
          .width(80)

        Text(this.licenseInfo.licenseType)
          .fontSize(16)
          .width('70%')
          .maxLines(1)
      }
      .width('100%')
      .height(40)

      Row() {
        Text('初次领证日期')
          .fontSize(16)
          .width(80)

        Text(this.licenseInfo.issueDate)
          .fontSize(16)
          .width('70%')
          .maxLines(1)
      }
      .width('100%')
      .height(40)

      Row() {
        Text('有效期限')
          .fontSize(16)
          .width(80)

        Text(`${this.licenseInfo.validFrom} 至 ${this.licenseInfo.validTo}`)
          .fontSize(16)
          .width('70%')
          .maxLines(1)
      }
      .width('100%')
      .height(40)

      // 底部按钮
      Button('查看详情')
        .width('90%')
        .height(45)
        .backgroundColor('#007DFF')
        .fontColor('#FFFFFF')
        .fontSize(18)
        .borderRadius(22.5)
        .margin({ top: 15 })

      Text('* 本电子驾照与实体驾照具有同等法律效力')
        .fontSize(12)
        .fontColor('#999999')
        .margin({ top: 10 })
    }
    .width('100%')
    .backgroundColor('#FFFFFF')
    .padding({ top: 10 })
  }
}