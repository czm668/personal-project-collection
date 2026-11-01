@Entry
@Component
export struct SearchView {
  @State searchText: string = '';
  @State searchResults: string[] = [];
  @State showEmptyTip: boolean = false;

  private search() {
    if (this.searchText.trim()) {
      this.showEmptyTip = false;
      this.searchResults = this.generateMockResults();
    } else {
      this.showEmptyTip = true;
      this.searchResults = [];
    }
  }

  private generateMockResults(): string[] {
    return [
      `${this.searchText} 相关 民警向茶农宣讲交通安全知识`,
      `${this.searchText} 相关 车辆高架桥上爆胎 铁骑及时换胎解困`,
      `${this.searchText} 相关 福建省安全形势稳定向好今年部署七 大重点工作`,
      `${this.searchText} 相关 福州交警进校园宣传交通安全 筑牢学 生出行安全防线`,
      `${this.searchText} 相关 左毅：30载坚守，群众称他“不下班的 警察”`
    ];
  }

  build() {
    Column({ space: 10 }) {
      TextInput({
        placeholder: '请输入搜索关键词',
        text: this.searchText
      })
        .width('90%')
        .height(40)
        .padding(8)
        .borderRadius(8)
        .borderWidth(1)
        .borderColor('#CCCCCC')
        .onChange((value: string) => {
          this.searchText = value;
        })
        .onSubmit(() => {
          this.search();
        })

      Button('开始搜索')
        .width(120)
        .height(36)
        .backgroundColor('#007AFF')
        .fontColor('white')
        .margin({ top: 10 })
        .onClick(() => {
          this.search();
        })

      if (this.searchResults.length > 0) {
        List({ space: 8 }) {
          ForEach(this.searchResults, (item: string) => {
            ListItem() {
              Text(item)
                .width('90%')
                .padding(10)
                .backgroundColor('#FFFFFF')
                .borderRadius(4)
            }
          })
        }
        .width('100%')
        .layoutWeight(1)
        .divider({ strokeWidth: 0.5, color: '#EEEEEE' })
      }

      if (this.showEmptyTip) {
        Text('请输入有效的搜索内容')
          .fontColor('red')
          .margin({ top: 10 })
      }

      if (!this.searchResults.length && !this.showEmptyTip) {
        Text('暂无搜索结果，尝试输入关键词进行搜索')
          .fontColor('#666666')
          .margin({ top: 20 })
      }
    }
    .width('100%')
    .height('100%')
    .padding(16)
    .backgroundColor('#F5F5F5')
  }
}