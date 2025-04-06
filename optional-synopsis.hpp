// fb8602f8-ef1d-49ce-94ed-a141adfe7a5c
template <class T> class optional<T &> {
      public:
        using value_type = T;
        using iterator = implementation_defined;

      public:
        // fb8602f8-ef1d-49ce-94ed-a141adfe7a5c end

        // 2f167cef-1888-487e-9fd2-c26cc27c3c92
        constexpr optional() noexcept = default;
        constexpr optional(nullopt_t) noexcept : optional() {}
        constexpr optional(const optional &rhs) nboexcept = default;
        // 2f167cef-1888-487e-9fd2-c26cc27c3c92 end

        // f6696e02-de4e-4200-bd98-1bb3027e2d72
        template <class Arg>
        constexpr explicit optional(in_place_t, Arg &&arg);
        template <class U>
        constexpr explicit(/*see below*/)
                optional(U &&u) noexcept(/*see below*/);

        template <class U>
        constexpr explicit(/*see below*/)
                optional(optional<U> &rhs) noexcept(/*see below*/);

        template <class U>
        constexpr explicit(/*see below*/)
                optional(const optional<U> &rhs) noexcept(/*see below*/);

        template <class U>
        constexpr explicit(/*see below*/)
                optional(optional<U> &&rhs) noexcept(/*see below*/);

        template <class U>
        constexpr explicit(/*see below*/)
                optional(const optional<U> &&rhs) noexcept(/*see below*/);
        // f6696e02-de4e-4200-bd98-1bb3027e2d72 end

        // b19ae341-c9bf-4ada-9c34-dfd955947cc9
        constexpr ~optional() = default;
        // b19ae341-c9bf-4ada-9c34-dfd955947cc9 end

        // 3109c3b6-fce3-42c1-88e9-be8b353aadb9
        constexpr optional &operator=(nullopt_t) noexcept;
        constexpr optional &operator=(const optional &rhs) noexcept = default;
        template <class U> constexpr T &emplace(U &&u) noexcept(/*see below*/);
        // 3109c3b6-fce3-42c1-88e9-be8b353aadb9 end

        // 904f4d4b-de24-459f-bc07-00a93a45c9dc
        constexpr void swap(optional &rhs) noexcept;
        // 904f4d4b-de24-459f-bc07-00a93a45c9dc end

        // 646f38bf-9b8b-482a-b68d-24297c1c2636
        constexpr iterator begin() const noexcept;
        constexpr iterator end() const noexcept;
        // 646f38bf-9b8b-482a-b68d-24297c1c2636 end

        // 5383e2b9-ee37-4c47-8cee-4dd143dd8d27
        constexpr T *operator->() const noexcept;
        constexpr T &operator*() const noexcept;
        constexpr explicit operator bool() const noexcept;
        constexpr bool has_value() const noexcept;
        constexpr T &value() const; // freestanding-deleted
        template <class U = remove_cv_t<T>>
        constexpr remove_cv_t<T> value_or(U &&u) const;
        // 5383e2b9-ee37-4c47-8cee-4dd143dd8d27 end

        // 4f2fafac-9479-47ce-8f15-983c315af300
        template <class F> constexpr auto and_then(F &&f) const;

        template <class F>
        constexpr optional<invoke_result_t<F, T &>> transform(F &&f) const;

        template <class F> constexpr optional or_else(F &&f) const;

        // 4f2fafac-9479-47ce-8f15-983c315af300 end

        // 62d12978-14b9-4ecf-afb0-e80e25062a3b
        constexpr void reset() noexcept;
        // 62d12978-14b9-4ecf-afb0-e80e25062a3b end

        // 339cf95f-5e8a-48b3-b745-7e40ae2eaa23
      private:
        T *val = nullptr; // exposition only

        template <class U>
        constexpr void convert_ref_init_val(U &&u); // exposition only
};
// 339cf95f-5e8a-48b3-b745-7e40ae2eaa23 end
