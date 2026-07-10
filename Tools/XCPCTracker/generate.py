import os
import yaml

def generate_markdown():
    current_dir = os.path.dirname(os.path.abspath(__file__))
    yaml_path = os.path.join(current_dir, "data.yaml")
    
    if not os.path.exists(yaml_path):
        print(f"❌ 找不到数据文件: {yaml_path}")
        return

    with open(yaml_path, "r", encoding="utf-8") as f:
        contests = yaml.safe_load(f) or []

    # ----------------------------------------------------
    # 1. 生成 INDEX.md
    # ----------------------------------------------------
    index_content = """# XCPC 赛站题评总览

> 本表格由脚本自动生成
> [知识点索引](./TAGS.md)

| 年份 | 赛站 | 出题组 | Easy<br>签到+铜快 | Medium<br>稳铜/银快 | Hard<br>稳银/金快 | Expert<br>稳金/更难的题（不完整） |
| :---: | :--- | :--- | :--- | :--- | :--- | :--- |
"""
    
    reverse_index = {}
    difficulties = ["Easy", "Medium", "Hard", "Expert"]
    
    last_year = None

    for c in contests:
        year = c.get("year", "")
        
        # 如果年份和上一行相同，则表格单元格留空
        if year == last_year:
            year_str = ""
        else:
            year_str = str(year) if year else ""
            last_year = year if year else last_year
            
        contest = c.get("contest", "")
        setter = c.get("setter", "")
        
        # 处理四个难度的单元格
        cells = {}
        for diff in difficulties:
            # 兼容处理
            problems_dict = c.get("problems") or {}
            tags = problems_dict.get(diff) or []
            
            if not tags:
                cells[diff] = "/"  # 空白默认填 /
            else:
                cells[diff] = "<br>".join(tags)
                
                # 构建反查索引
                for tag in tags:
                    if tag in ["/", "(暂未整理)"]:
                        continue
                    if tag not in reverse_index:
                        reverse_index[tag] = []
                    reverse_index[tag].append({
                        "year": year,
                        "contest": contest,
                        "difficulty": diff
                    })

        index_content += f"| {year_str} | {contest} | {setter} | {cells['Easy']} | {cells['Medium']} | {cells['Hard']} | {cells['Expert']} |\n"

    # 写入 INDEX.md
    with open(os.path.join(current_dir, "INDEX.md"), "w", encoding="utf-8") as f:
        f.write(index_content)

    # ----------------------------------------------------
    # 2. 生成 TAGS.md
    # ----------------------------------------------------
    tags_content = "# 知识点索引\n\n"
    tags_content += "[返回总览表](./INDEX.md)\n\n"
    
    # 按照知识点名字拼音/字母排序
    for tag in sorted(reverse_index.keys()):
        # 计算当前知识点出现的频次（即题目数量）
        tag_freq = len(reverse_index[tag])
        
        # 在标题后面加上频次展示
        tags_content += f"### {tag} ({tag_freq})\n"

        for item in reverse_index[tag]:
            # 将英文的 key 转换为更易读的中文描述
            diff_cn = {
                "Easy": "Easy",
                "Medium": "Medium",
                "Hard": "Hard",
                "Expert": "Expert"
            }.get(item['difficulty'], item['difficulty'])
            
            tags_content += f"- {item['year']} {item['contest']} ({diff_cn})\n"
        tags_content += "\n"

    # 写入 TAGS.md
    with open(os.path.join(current_dir, "TAGS.md"), "w", encoding="utf-8") as f:
        f.write(tags_content)

    print("成功更新 INDEX.md 和 TAGS.md")

if __name__ == "__main__":
    generate_markdown()