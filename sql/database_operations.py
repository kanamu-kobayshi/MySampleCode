
from sqlalchemy import create_engine, Column, Integer, String, Float, Sequence
from sqlalchemy.orm import declarative_base, sessionmaker
from sqlalchemy import func


# データベースの接続設定
DB_PATH = 'db/example.db'
engine = create_engine(f'sqlite:///{DB_PATH}')
Session = sessionmaker(bind=engine)
session = Session()

# Userテーブルの定義
Base = declarative_base()


class User(Base):
    __tablename__ = 'users'
    id = Column(Integer, Sequence('user_id_seq'), primary_key=True)
    name = Column(String(50))
    height = Column(Float)
    weight = Column(Float)

# ユーザーのデータを挿入する関数


def insert_user(name, height, weight):
    user = User(name=name, height=height, weight=weight)
    session.add(user)
    session.commit()

# 指定したユーザーIDのデータを削除する関数


def delete_user(user_id):
    user = session.query(User).filter_by(id=user_id).first()
    if user:
        session.delete(user)
        session.commit()

# 指定したユーザーIDのデータを更新する関数


def update_user(user_id, name=None, height=None, weight=None):
    user = session.query(User).filter_by(id=user_id).first()
    if user:
        if name:
            user.name = name
        if height:
            user.height = height
        if weight:
            user.weight = weight
        session.commit()

####################################################################################################

# 1. 全てのレコードを取得


def get_all_users():
    return session.query(User).all()

# 2. 特定の条件を満たすレコードを取得


def get_users_by_condition(column, condition, value):
    column_mapping = {
        "height": User.height,
        "weight": User.weight
    }

    if column not in column_mapping:
        return []

    if condition == "equal":
        filter_condition = column_mapping[column] == value
    elif condition == "greater_than_or_equal":
        filter_condition = column_mapping[column] >= value
    elif condition == "less_than":
        filter_condition = column_mapping[column] < value
    elif condition == "less_than_or_equal":
        filter_condition = column_mapping[column] <= value
    else:
        return []

    return session.query(User).filter(filter_condition).all()

# 3. 順序を指定してレコードを取得


def get_users_ordered_by_height():
    return session.query(User).order_by(User.height.desc()).all()

# 4. 制限付きでレコードを取得


def get_limited_users(limit=5):
    return session.query(User).limit(limit).all()

# 5. 特定のカラムのみを取得


def get_selected_columns(columns):
    column_mapping = {
        "id": User.id,
        "name": User.name,
        "height": User.height,
        "weight": User.weight
    }

    selected_columns = [column_mapping[col]
                        for col in columns if col in column_mapping]
    return session.query(*selected_columns).all()
# 6. 集計関数を使用


def get_average_height():
    avg_height = session.query(func.avg(User.height)).scalar()
    return avg_height
