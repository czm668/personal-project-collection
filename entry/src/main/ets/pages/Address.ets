import { router } from "@kit.ArkUI";

interface AddressItem {
  id: number;
  name: string;
  phone: string;
  address: string;
  isDefault: boolean;
}

@Entry
@Component
export struct Address {
  @State addressList: AddressItem[] = [
    {
      id: 1,
      name: '张三',
      phone: '138****1234',
      address: '北京市朝阳区建国路88号',
      isDefault: true
    } as AddressItem,
    {
      id: 2,
      name: '张三',
      phone: '139****5678',
      address: '上海市浦东新区张江高科技园区',
      isDefault: false
    } as AddressItem
  ];

  @State currentId: number = 0;
  @State name: string = '';
  @State phone: string = '';
  @State address: string = '';
  @State isDefault: boolean = false;
  @State isAdding: boolean = false;
  @State isEditing: boolean = false;
  @State showDeleteDialog: boolean = false;
  @State deletingId: number = 0;

  build(): void {
    Column() {
      Row() {
        Image($r("app.media.back"))
          .size({ width: 24, height: 24 })
          .margin({ right: 10 })
          .onClick(() => router.back())

        Text('我的地址')
          .fontSize(20)
          .fontWeight(FontWeight.Bold)

        Text('')
          .layoutWeight(1)
      }
      .width('100%')
      .height(56)
      .padding({ left: 15, right: 15 })
      .backgroundColor(0xFFFFFF)

      ForEach(this.addressList, (item: AddressItem) => {
        Column() {
          Column() {
            Row() {
              Text(item.name)
                .fontSize(16)
                .fontColor(0x333333)
                .margin({ right: 20 })

              Text(item.phone)
                .fontSize(16)
                .fontColor(0x333333)

              if (item.isDefault) {
                Text('默认')
                  .fontSize(12)
                  .fontColor(0xFFFFFF)
                  .backgroundColor(0xFF9500)
                  .padding({ left: 5, right: 5, top: 1, bottom: 1 })
                  .borderRadius(3)
                  .margin({ left: 10 })
              }
            }
            .width('100%')
            .margin({ bottom: 8 })

            Text(item.address)
              .fontSize(14)
              .fontColor(0x666666)
              .maxLines(2)
          }
          .width('100%')
          .padding({ left: 15, right: 15, top: 15, bottom: 15 })

          Row() {
            Row() {
              Image($r("app.media.edit"))
                .size({ width: 18, height: 18 })
                .margin({ right: 5 })

              Text('编辑')
                .fontSize(14)
                .fontColor(0x666666)
            }
            .onClick(() => this.handleEdit(item))

            Text('')
              .layoutWeight(1)

            Row() {
              Image($r("app.media.delete"))
                .size({ width: 18, height: 18 })
                .margin({ right: 5 })

              Text('删除')
                .fontSize(14)
                .fontColor(0x666666)
            }
            .onClick(() => this.handleDelete(item.id))
          }
          .width('100%')
          .height(40)
          .padding({ left: 15, right: 15 })
          .backgroundColor(0xF9F9F9)
        }
        .width('100%')
        .backgroundColor(0xFFFFFF)
        .margin({ bottom: 15 })
      })

      Button('+ 添加新地址')
        .width('90%')
        .height(50)
        .backgroundColor(0x007DFF)
        .fontColor(0xFFFFFF)
        .fontSize(18)
        .margin({ top: 20, bottom: 20 })
        .onClick(() => this.handleAdd())

      if (this.isAdding || this.isEditing) {
        Column() {
          Text(this.isAdding ? '添加新地址' : '编辑地址')
            .fontSize(18)
            .fontWeight(FontWeight.Bold)
            .margin({ bottom: 20 })
            .width('100%')
            .textAlign(TextAlign.Center)

          Column() {
            if (!this.name) {
              Text('请输入姓名')
                .fontColor(0xCCCCCC)
            }
            TextInput()
              .width('100%')
              .margin({ bottom: 15 })
              .onChange((value: string) => { this.name = value; })
          }

          Column() {
            if (!this.phone) {
              Text('请输入手机号码')
                .fontColor(0xCCCCCC)
            }
            TextInput()
              .width('100%')
              .margin({ bottom: 15 })
              .onChange((value: string) => { this.phone = value; })
          }

          Column() {
            if (!this.address) {
              Text('请输入详细地址')
                .fontColor(0xCCCCCC)
            }
            TextInput()
              .width('100%')
              .margin({ bottom: 15 })
              .onChange((value: string) => { this.address = value; })
          }

          Row() {
            Text('设为默认地址')
              .fontSize(14)
              .fontColor(0x333333)
            Button(this.isDefault ? '已设为默认' : '设为默认')
              .backgroundColor(this.isDefault ? 0x007DFF : 0xE5E5E5)
              .fontColor(this.isDefault ? 0xFFFFFF : 0x333333)
              .onClick(() => {
                this.isDefault = !this.isDefault;
              })
          }
          .width('100%')
          .margin({ bottom: 20 })

          Row() {
            Button(this.isAdding ? '添加' : '保存')
              .width('45%')
              .height(40)
              .backgroundColor(0x007DFF)
              .fontColor(0xFFFFFF)
              .onClick(() => {
                if (this.isAdding) {
                  this.addNewAddress();
                } else {
                  this.updateAddress();
                }
              })

            Button('取消')
              .width('45%')
              .height(40)
              .backgroundColor(0xE5E5E5)
              .fontColor(0x333333)
              .margin({ left: 10 })
              .onClick(() => { this.resetForm(); })
          }
          .width('100%')
        }
        .width('90%')
        .padding(20)
        .backgroundColor(0xFFFFFF)
        .borderRadius(10)
      }

      if (this.showDeleteDialog) {
        Column() {
          Text('确认删除')
            .fontSize(18)
            .fontWeight(FontWeight.Bold)
            .margin({ bottom: 10 })
          Text('您确定要删除这个地址吗？')
            .fontSize(14)
            .margin({ bottom: 20 })
          Row() {
            Button('删除')
              .width('45%')
              .height(40)
              .backgroundColor(0xFF4444)
              .fontColor(0xFFFFFF)
              .onClick(() => {
                this.confirmDelete();
              })
            Button('取消')
              .width('45%')
              .height(40)
              .backgroundColor(0xE5E5E5)
              .fontColor(0x333333)
              .margin({ left: 10 })
              .onClick(() => {
                this.showDeleteDialog = false;
              })
          }
        }
        .width('80%')
        .padding(20)
        .backgroundColor(0xFFFFFF)
        .borderRadius(10)
        .margin({ top: 50, left: 50, right: 50, bottom: 50 })
      }
    }
    .width('100%')
    .height('100%')
    .backgroundColor(0xF5F5F5)
    .padding({ top: 15 });
  }

  handleAdd(): void {
    this.resetForm();
    this.isAdding = true;
  }

  handleEdit(item: AddressItem): void {
    this.currentId = item.id;
    this.name = item.name;
    this.phone = item.phone;
    this.address = item.address;
    this.isDefault = item.isDefault;
    this.isEditing = true;
  }

  handleDelete(id: number): void {
    this.deletingId = id;
    this.showDeleteDialog = true;
  }

  resetForm(): void {
    this.currentId = 0;
    this.name = '';
    this.phone = '';
    this.address = '';
    this.isDefault = false;
    this.isAdding = false;
    this.isEditing = false;
    this.showDeleteDialog = false;
  }

  addNewAddress(): void {
    if (!this.name || !this.phone || !this.address) {
      console.error('请填写完整地址信息');
      return;
    }

    const newId = this.addressList.length > 0 ? Math.max(...this.addressList.map(item => item.id)) + 1 : 1;

    if (this.isDefault) {
      this.addressList = this.addressList.map(item => {
        return {
          id: item.id,
          name: item.name,
          phone: item.phone,
          address: item.address,
          isDefault: false
        } as AddressItem;
      });
    }

    this.addressList = this.addressList.concat({
      id: newId,
      name: this.name,
      phone: this.phone,
      address: this.address,
      isDefault: this.isDefault
    } as AddressItem);

    this.resetForm();
    console.info('添加新地址成功');
  }

  updateAddress(): void {
    if (!this.name || !this.phone || !this.address) {
      console.error('请填写完整地址信息');
      return;
    }

    if (this.isDefault) {
      this.addressList = this.addressList.map(item => {
        return {
          id: item.id,
          name: item.name,
          phone: item.phone,
          address: item.address,
          isDefault: item.id === this.currentId
        } as AddressItem;
      });
    }

    this.addressList = this.addressList.map(item => {
      if (item.id === this.currentId) {
        return {
          id: item.id,
          name: this.name,
          phone: this.phone,
          address: this.address,
          isDefault: this.isDefault
        } as AddressItem;
      }
      return item;
    });

    this.resetForm();
    console.info('更新地址成功');
  }

  confirmDelete(): void {
    this.addressList = this.addressList.filter(item => item.id !== this.deletingId);
    this.showDeleteDialog = false;
    console.info('删除地址成功');
  }
}