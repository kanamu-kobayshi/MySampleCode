from database_operations import (insert_user, delete_user, update_user,
                                 get_all_users, get_users_by_condition,
                                 get_users_ordered_by_height, get_limited_users,
                                 get_selected_columns, get_average_height)

# データを挿入
# insert_user(name, height, weight)
insert_user()

# データを削除
# delete_user(user_id)
delete_user()

# データを更新
# update_user(user_id, name=None, height=None, weight=None)
update_user()

#########################################################################
print("################################")

# 全てのユーザーを取得して表示

users = get_all_users()
for user in users:
    print(user.id, user.name, user.height, user.weight)

print("################################")


# 身長が170cm以上のユーザーを取得して表示

users = get_users_by_condition("height", "greater_than_or_equal", 170)
for user in users:
    print(user.id, user.name, user.height, user.weight)

'''
"height": User.height
"weight": User.weight

"equal": == 等しい
"greater_than_or_equal": >= 以上
"less_than_or_equal": <= 以下
"less_than": < 未満
'''
print("################################")

# 身長の降順でユーザーを取得して表示

ordered_users = get_users_ordered_by_height()
for user in ordered_users:
    print(user.id, user.name, user.height, user.weight)

print("################################")


# 最初の5人のユーザーを取得して表示

limited_users = get_limited_users(limit=5)
for user in limited_users:
    print(user.id, user.name, user.height, user.weight)

print("################################")


# 特定のカラムのみを取得して表示

columns = get_selected_columns(["name"])
for column, in columns:
    print(column)

'''
"id": User.id
"name": User.name
"height": User.height
"weight": User.weight
'''
print("################################")


# 全ユーザーの平均身長を取得して表示

avg_height = get_average_height()
print("Average Height:", avg_height)

print("################################")
