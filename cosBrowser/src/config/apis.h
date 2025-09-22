#ifndef APIS_H
#define APIS_H

namespace API {
namespace LOGIN {
[[maybe_unused]] static constexpr int NORMAL = 1; // 普通登录
}

namespace BUCKETS {
[[maybe_unused]] static constexpr int BASE = 100;
[[maybe_unused]] static constexpr int LIST = BASE + 1; // 获取存储桶列表
[[maybe_unused]] static constexpr int PUT = BASE + 2; // 创建存储桶
[[maybe_unused]] static constexpr int DEL = BASE + 3; // 删除存储桶
}

namespace OBJECTS {
[[maybe_unused]] static constexpr int BASE = 200;
[[maybe_unused]] static constexpr int LIST = BASE + 1; // 获取对象列表
[[maybe_unused]] static constexpr int GET = BASE + 10; // 上传对象
[[maybe_unused]] static constexpr int PUT = BASE + 20; // 下载对象
}
}

#endif // APIS_H
