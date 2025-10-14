#include "text_screen.hpp"

TextScreen::TextScreen(const std::vector<std::string>& lines)
    : lines_(lines), scroll_position_(0) {}

Component TextScreen::get_component() {
    // Создаём контейнер, чтобы FTXUI мог обрабатывать события
    auto container = Container::Vertical({});

    auto renderer = Renderer(container, [&] {
        Elements displayed;

        int start = scroll_position_;
        int end = std::min<int>(scroll_position_ + max_visible_, (int)lines_.size());

        for (int i = start; i < end; ++i)
            displayed.push_back(text(lines_[i]));

        return vbox(displayed) | frame | flex;
    });

    // Вешаем обработку событий на контейнер через Renderer
    auto with_events = Container::Vertical({renderer});
    with_events |= CatchEvent([&](Event event) {
        if (event == Event::ArrowUpCtrl) {
            if (scroll_position_ > 0) scroll_position_--;
            return true;
        }
        if (event == Event::ArrowDownCtrl) {
            if (scroll_position_ + max_visible_ < (int)lines_.size()) scroll_position_++;
            return true;
        }
        return false;
    });

    return with_events;
}
