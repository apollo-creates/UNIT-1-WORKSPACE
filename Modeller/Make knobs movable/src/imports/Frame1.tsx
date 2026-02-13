import svgPaths from "./svg-jlepp2ol9g";

function MainWindow() {
  return (
    <div className="absolute contents left-0 top-0" data-name="Main window">
      <div className="absolute bg-[#292929] h-[800px] left-0 top-0 w-[1545px]" data-name="Background" />
      <div className="absolute bg-[#b9b9b9] h-[45px] left-0 top-[755px] w-[1545px]" data-name="Footer" />
      <div className="absolute bg-[#b9b9b9] h-[57px] left-0 top-0 w-[1545px]" data-name="Header" />
      <p className="absolute font-['Manrope:Light',sans-serif] font-light leading-[normal] left-[1362px] text-[32px] text-[rgba(97,97,97,0.74)] top-[754px]">ALPHA</p>
      <div className="absolute font-['Inter:Extra_Light',sans-serif] font-extralight leading-[normal] left-[1465px] not-italic text-[12px] text-[rgba(6,6,6,0.49)] top-[762px] whitespace-nowrap">
        <p className="mb-0">Music</p>
        <p>Technology</p>
      </div>
      <div className="absolute bg-[#373737] h-[697px] left-0 top-[57px] w-[40px]" data-name="Left  meter panel" />
      <div className="absolute bg-[#373737] h-[698px] left-[1505px] top-[57px] w-[40px]" data-name="Right Meter Panel" />
      <div className="absolute flex h-[680px] items-center justify-center left-[369px] top-[64px] w-0" style={{ "--transform-inner-width": "1185", "--transform-inner-height": "153" } as React.CSSProperties}>
        <div className="flex-none rotate-90">
          <div className="h-0 relative w-[680px]" data-name="Divider 1">
            <div className="absolute inset-[-1.5px_0_0_0]">
              <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 680 1.5">
                <line id="Divider 1" stroke="var(--stroke-0, #5B5B5B)" strokeLinecap="round" strokeOpacity="0.46" strokeWidth="1.5" x1="0.75" x2="679.25" y1="0.75" y2="0.75" />
              </svg>
            </div>
          </div>
        </div>
      </div>
      <div className="absolute flex h-[678.5px] items-center justify-center left-[1256.5px] top-[63.61px] w-0" style={{ "--transform-inner-width": "1185", "--transform-inner-height": "153" } as React.CSSProperties}>
        <div className="flex-none rotate-90">
          <div className="h-0 relative w-[678.5px]" data-name="Divider 2">
            <div className="absolute inset-[-0.75px_-0.11%]">
              <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 680 1.5">
                <path d="M0.75 0.75H679.25" id="Divider 2" stroke="var(--stroke-0, #5B5B5B)" strokeLinecap="round" strokeOpacity="0.46" strokeWidth="1.5" />
              </svg>
            </div>
          </div>
        </div>
      </div>
      <p className="absolute font-['Manrope:Light',sans-serif] font-light leading-[normal] left-[1349px] text-[33px] text-white top-[66px]">CAB</p>
      <p className="absolute font-['Manrope:Light',sans-serif] font-light leading-[normal] left-[18px] text-[#616161] text-[33px] top-[6px]">UNIT 1 - MODELLER</p>
    </div>
  );
}

function Knob() {
  return (
    <div className="absolute left-[109px] size-[70px] top-[247px]" data-name="Knob 1">
      <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 70">
        <g id="Knob 1">
          <g id="Ellipse 6">
            <mask fill="white" id="path-1-inside-1_1_109">
              <path d={svgPaths.p2b296300} />
            </mask>
            <path d={svgPaths.p2b296300} fill="var(--fill-0, #4D4D4D)" mask="url(#path-1-inside-1_1_109)" stroke="var(--stroke-0, #ABABAB)" />
          </g>
          <g id="Group 1">
            <circle cx="35.25" cy="35.25" fill="var(--fill-0, #4D4D4D)" id="Ellipse 5" r="25.75" stroke="var(--stroke-0, #C8C8C8)" />
            <g filter="url(#filter0_d_1_109)" id="Line 1">
              <line stroke="var(--stroke-0, #E2E2E2)" strokeLinecap="round" strokeWidth="2" x1="35" x2="35" y1="20.7279" y2="10" />
            </g>
          </g>
        </g>
        <defs>
          <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="20.7279" id="filter0_d_1_109" width="10" x="30" y="9">
            <feFlood floodOpacity="0" result="BackgroundImageFix" />
            <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
            <feOffset dy="4" />
            <feGaussianBlur stdDeviation="2" />
            <feComposite in2="hardAlpha" operator="out" />
            <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.25 0" />
            <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_109" />
            <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_109" mode="normal" result="shape" />
          </filter>
        </defs>
      </svg>
    </div>
  );
}

function Knob1() {
  return (
    <div className="absolute left-[229px] size-[70px] top-[247px]" data-name="Knob 2">
      <div className="absolute inset-[0_0_-2.64%_0]">
        <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 71.85">
          <g id="Knob 2">
            <g id="Ellipse 6">
              <mask fill="white" id="path-1-inside-1_1_105">
                <path d={svgPaths.p2b296300} />
              </mask>
              <path d={svgPaths.p2b296300} fill="var(--fill-0, #4D4D4D)" mask="url(#path-1-inside-1_1_105)" stroke="var(--stroke-0, #ABABAB)" />
            </g>
            <g filter="url(#filter0_d_1_105)" id="Ellipse 5">
              <circle cx="35" cy="35" fill="var(--fill-0, #4D4D4D)" r="26.25" />
              <circle cx="35" cy="35" r="25.75" stroke="var(--stroke-0, #C8C8C8)" />
            </g>
          </g>
          <defs>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="65.7" id="filter0_d_1_105" width="65.7" x="2.15002" y="6.15">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="4" />
              <feGaussianBlur stdDeviation="3.3" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.13 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_105" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_105" mode="normal" result="shape" />
            </filter>
          </defs>
        </svg>
      </div>
    </div>
  );
}

function Knob2() {
  return (
    <div className="absolute left-[109px] size-[70px] top-[367px]" data-name="Knob 3">
      <div className="absolute inset-[0_0_-2.64%_0]">
        <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 71.85">
          <g id="Knob 2">
            <g id="Ellipse 6">
              <mask fill="white" id="path-1-inside-1_1_105">
                <path d={svgPaths.p2b296300} />
              </mask>
              <path d={svgPaths.p2b296300} fill="var(--fill-0, #4D4D4D)" mask="url(#path-1-inside-1_1_105)" stroke="var(--stroke-0, #ABABAB)" />
            </g>
            <g filter="url(#filter0_d_1_105)" id="Ellipse 5">
              <circle cx="35" cy="35" fill="var(--fill-0, #4D4D4D)" r="26.25" />
              <circle cx="35" cy="35" r="25.75" stroke="var(--stroke-0, #C8C8C8)" />
            </g>
          </g>
          <defs>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="65.7" id="filter0_d_1_105" width="65.7" x="2.15002" y="6.15">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="4" />
              <feGaussianBlur stdDeviation="3.3" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.13 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_105" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_105" mode="normal" result="shape" />
            </filter>
          </defs>
        </svg>
      </div>
    </div>
  );
}

function Knob3() {
  return (
    <div className="absolute left-[229px] size-[70px] top-[367px]" data-name="Knob 4">
      <div className="absolute inset-[0_0_-2.64%_0]">
        <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 71.85">
          <g id="Knob 2">
            <g id="Ellipse 6">
              <mask fill="white" id="path-1-inside-1_1_105">
                <path d={svgPaths.p2b296300} />
              </mask>
              <path d={svgPaths.p2b296300} fill="var(--fill-0, #4D4D4D)" mask="url(#path-1-inside-1_1_105)" stroke="var(--stroke-0, #ABABAB)" />
            </g>
            <g filter="url(#filter0_d_1_105)" id="Ellipse 5">
              <circle cx="35" cy="35" fill="var(--fill-0, #4D4D4D)" r="26.25" />
              <circle cx="35" cy="35" r="25.75" stroke="var(--stroke-0, #C8C8C8)" />
            </g>
          </g>
          <defs>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="65.7" id="filter0_d_1_105" width="65.7" x="2.15002" y="6.15">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="4" />
              <feGaussianBlur stdDeviation="3.3" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.13 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_105" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_105" mode="normal" result="shape" />
            </filter>
          </defs>
        </svg>
      </div>
    </div>
  );
}

function Pedal() {
  return (
    <div className="absolute contents left-[90px] top-[213px]" data-name="Pedal">
      <div className="absolute h-[395px] left-[90px] rounded-[25px] top-[213px] w-[229px]" data-name="Pedal Body" style={{ backgroundImage: "linear-gradient(200.145deg, rgb(241, 241, 241) 4.5142%, rgb(235, 235, 235) 98.784%)" }}>
        <div aria-hidden="true" className="absolute border-4 border-[#dadada] border-solid inset-[-2px] pointer-events-none rounded-[27px] shadow-[0px_4px_4px_0px_rgba(0,0,0,0.25),0px_4px_8.3px_4px_rgba(0,0,0,0.23)]" />
      </div>
      <div className="absolute left-[169px] size-[70px] top-[510px]" data-name="Pedal Glow Ring">
        <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 70">
          <circle cx="35" cy="35" id="Pedal Glow Ring" r="32.5" stroke="var(--stroke-0, #4D4D4D)" strokeOpacity="0.98" strokeWidth="5" />
        </svg>
      </div>
      <div className="absolute left-[179px] size-[50px] top-[520px]" data-name="Pedal Glow Ring">
        <div className="absolute inset-[-4%]">
          <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 54 54">
            <g id="Pedal Glow Ring">
              <circle cx="27" cy="27" fill="url(#paint0_linear_1_103)" r="25" />
              <circle cx="27" cy="27" r="26" stroke="var(--stroke-0, #6F6F6F)" strokeOpacity="0.98" strokeWidth="2" />
            </g>
            <defs>
              <linearGradient gradientUnits="userSpaceOnUse" id="paint0_linear_1_103" x1="27" x2="27" y1="2" y2="52">
                <stop stopColor="#6C6C6C" />
                <stop offset="1" stopColor="#4B4B4B" />
              </linearGradient>
            </defs>
          </svg>
        </div>
      </div>
      <p className="absolute font-['Manrope:ExtraLight',sans-serif] font-extralight leading-[normal] left-[173px] text-[24px] text-black top-[326px]">DRIVE</p>
      <Knob />
      <Knob1 />
      <Knob2 />
      <Knob3 />
    </div>
  );
}

function Knob4() {
  return (
    <div className="absolute left-[1130px] size-[70px] top-[500px]" data-name="Knob 5">
      <div className="absolute inset-[0_0_-2.64%_0]">
        <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 71.85">
          <g id="Knob 2">
            <g id="Ellipse 6">
              <mask fill="white" id="path-1-inside-1_1_105">
                <path d={svgPaths.p2b296300} />
              </mask>
              <path d={svgPaths.p2b296300} fill="var(--fill-0, #4D4D4D)" mask="url(#path-1-inside-1_1_105)" stroke="var(--stroke-0, #ABABAB)" />
            </g>
            <g filter="url(#filter0_d_1_105)" id="Ellipse 5">
              <circle cx="35" cy="35" fill="var(--fill-0, #4D4D4D)" r="26.25" />
              <circle cx="35" cy="35" r="25.75" stroke="var(--stroke-0, #C8C8C8)" />
            </g>
          </g>
          <defs>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="65.7" id="filter0_d_1_105" width="65.7" x="2.15002" y="6.15">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="4" />
              <feGaussianBlur stdDeviation="3.3" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.13 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_105" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_105" mode="normal" result="shape" />
            </filter>
          </defs>
        </svg>
      </div>
    </div>
  );
}

function Knob5() {
  return (
    <div className="absolute left-[434px] size-[70px] top-[500px]" data-name="Knob 6">
      <div className="absolute inset-[0_0_-2.64%_0]">
        <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 71.85">
          <g id="Knob 2">
            <g id="Ellipse 6">
              <mask fill="white" id="path-1-inside-1_1_105">
                <path d={svgPaths.p2b296300} />
              </mask>
              <path d={svgPaths.p2b296300} fill="var(--fill-0, #4D4D4D)" mask="url(#path-1-inside-1_1_105)" stroke="var(--stroke-0, #ABABAB)" />
            </g>
            <g filter="url(#filter0_d_1_105)" id="Ellipse 5">
              <circle cx="35" cy="35" fill="var(--fill-0, #4D4D4D)" r="26.25" />
              <circle cx="35" cy="35" r="25.75" stroke="var(--stroke-0, #C8C8C8)" />
            </g>
          </g>
          <defs>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="65.7" id="filter0_d_1_105" width="65.7" x="2.15002" y="6.15">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="4" />
              <feGaussianBlur stdDeviation="3.3" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.13 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_105" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_105" mode="normal" result="shape" />
            </filter>
          </defs>
        </svg>
      </div>
    </div>
  );
}

function Knob6() {
  return (
    <div className="absolute left-[521px] size-[70px] top-[500px]" data-name="Knob 7">
      <div className="absolute inset-[0_0_-2.64%_0]">
        <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 71.85">
          <g id="Knob 2">
            <g id="Ellipse 6">
              <mask fill="white" id="path-1-inside-1_1_105">
                <path d={svgPaths.p2b296300} />
              </mask>
              <path d={svgPaths.p2b296300} fill="var(--fill-0, #4D4D4D)" mask="url(#path-1-inside-1_1_105)" stroke="var(--stroke-0, #ABABAB)" />
            </g>
            <g filter="url(#filter0_d_1_105)" id="Ellipse 5">
              <circle cx="35" cy="35" fill="var(--fill-0, #4D4D4D)" r="26.25" />
              <circle cx="35" cy="35" r="25.75" stroke="var(--stroke-0, #C8C8C8)" />
            </g>
          </g>
          <defs>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="65.7" id="filter0_d_1_105" width="65.7" x="2.15002" y="6.15">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="4" />
              <feGaussianBlur stdDeviation="3.3" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.13 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_105" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_105" mode="normal" result="shape" />
            </filter>
          </defs>
        </svg>
      </div>
    </div>
  );
}

function Knob7() {
  return (
    <div className="absolute left-[608px] size-[70px] top-[500px]" data-name="Knob 8">
      <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 70">
        <g id="Knob 8">
          <g id="Ellipse 6">
            <mask fill="white" id="path-1-inside-1_1_100">
              <path d={svgPaths.p2b296300} />
            </mask>
            <path d={svgPaths.p2b296300} fill="var(--fill-0, #4D4D4D)" mask="url(#path-1-inside-1_1_100)" stroke="var(--stroke-0, #ABABAB)" />
          </g>
        </g>
      </svg>
    </div>
  );
}

function Knob8() {
  return (
    <div className="absolute left-[695px] size-[70px] top-[500px]" data-name="Knob 9">
      <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 70">
        <g id="Knob 8">
          <g id="Ellipse 6">
            <mask fill="white" id="path-1-inside-1_1_100">
              <path d={svgPaths.p2b296300} />
            </mask>
            <path d={svgPaths.p2b296300} fill="var(--fill-0, #4D4D4D)" mask="url(#path-1-inside-1_1_100)" stroke="var(--stroke-0, #ABABAB)" />
          </g>
        </g>
      </svg>
    </div>
  );
}

function Knob9() {
  return (
    <div className="absolute left-[782px] size-[70px] top-[500px]" data-name="Knob 10">
      <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 70">
        <g id="Knob 8">
          <g id="Ellipse 6">
            <mask fill="white" id="path-1-inside-1_1_100">
              <path d={svgPaths.p2b296300} />
            </mask>
            <path d={svgPaths.p2b296300} fill="var(--fill-0, #4D4D4D)" mask="url(#path-1-inside-1_1_100)" stroke="var(--stroke-0, #ABABAB)" />
          </g>
        </g>
      </svg>
    </div>
  );
}

function Knob10() {
  return (
    <div className="absolute left-[869px] size-[70px] top-[500px]" data-name="Knob 11">
      <div className="absolute inset-[0_0_-2.64%_0]">
        <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 71.85">
          <g id="Knob 2">
            <g id="Ellipse 6">
              <mask fill="white" id="path-1-inside-1_1_105">
                <path d={svgPaths.p2b296300} />
              </mask>
              <path d={svgPaths.p2b296300} fill="var(--fill-0, #4D4D4D)" mask="url(#path-1-inside-1_1_105)" stroke="var(--stroke-0, #ABABAB)" />
            </g>
            <g filter="url(#filter0_d_1_105)" id="Ellipse 5">
              <circle cx="35" cy="35" fill="var(--fill-0, #4D4D4D)" r="26.25" />
              <circle cx="35" cy="35" r="25.75" stroke="var(--stroke-0, #C8C8C8)" />
            </g>
          </g>
          <defs>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="65.7" id="filter0_d_1_105" width="65.7" x="2.15002" y="6.15">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="4" />
              <feGaussianBlur stdDeviation="3.3" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.13 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_105" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_105" mode="normal" result="shape" />
            </filter>
          </defs>
        </svg>
      </div>
    </div>
  );
}

function Knob11() {
  return (
    <div className="absolute left-[956px] size-[70px] top-[500px]" data-name="Knob 12">
      <div className="absolute inset-[0_0_-2.64%_0]">
        <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 71.85">
          <g id="Knob 2">
            <g id="Ellipse 6">
              <mask fill="white" id="path-1-inside-1_1_105">
                <path d={svgPaths.p2b296300} />
              </mask>
              <path d={svgPaths.p2b296300} fill="var(--fill-0, #4D4D4D)" mask="url(#path-1-inside-1_1_105)" stroke="var(--stroke-0, #ABABAB)" />
            </g>
            <g filter="url(#filter0_d_1_105)" id="Ellipse 5">
              <circle cx="35" cy="35" fill="var(--fill-0, #4D4D4D)" r="26.25" />
              <circle cx="35" cy="35" r="25.75" stroke="var(--stroke-0, #C8C8C8)" />
            </g>
          </g>
          <defs>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="65.7" id="filter0_d_1_105" width="65.7" x="2.15002" y="6.15">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="4" />
              <feGaussianBlur stdDeviation="3.3" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.13 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_105" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_105" mode="normal" result="shape" />
            </filter>
          </defs>
        </svg>
      </div>
    </div>
  );
}

function Knob12() {
  return (
    <div className="absolute left-[1043px] size-[70px] top-[500px]" data-name="Knob 13">
      <div className="absolute inset-[0_0_-2.64%_0]">
        <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 71.85">
          <g id="Knob 2">
            <g id="Ellipse 6">
              <mask fill="white" id="path-1-inside-1_1_105">
                <path d={svgPaths.p2b296300} />
              </mask>
              <path d={svgPaths.p2b296300} fill="var(--fill-0, #4D4D4D)" mask="url(#path-1-inside-1_1_105)" stroke="var(--stroke-0, #ABABAB)" />
            </g>
            <g filter="url(#filter0_d_1_105)" id="Ellipse 5">
              <circle cx="35" cy="35" fill="var(--fill-0, #4D4D4D)" r="26.25" />
              <circle cx="35" cy="35" r="25.75" stroke="var(--stroke-0, #C8C8C8)" />
            </g>
          </g>
          <defs>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="65.7" id="filter0_d_1_105" width="65.7" x="2.15002" y="6.15">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="4" />
              <feGaussianBlur stdDeviation="3.3" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.13 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_105" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_105" mode="normal" result="shape" />
            </filter>
          </defs>
        </svg>
      </div>
    </div>
  );
}

function Amp() {
  return (
    <div className="absolute contents left-[404px] top-[213px]" data-name="Amp">
      <div className="absolute h-[395px] left-[404px] rounded-[13px] top-[213px] w-[825px]" data-name="Amp Body">
        <div aria-hidden="true" className="absolute border-4 border-[rgba(63,63,63,0.42)] border-solid inset-[-2px] pointer-events-none rounded-[15px] shadow-[0px_4px_4px_0px_rgba(0,0,0,0.25),0px_4px_8.3px_4px_rgba(0,0,0,0.23)]" />
      </div>
      <div className="absolute bg-[#e9e9e9] h-[135px] left-[417px] rounded-bl-[5px] rounded-br-[5px] top-[461px] w-[800px]" />
      <div className="absolute bg-[rgba(66,66,66,0.84)] h-[235px] left-[417px] rounded-tl-[8px] rounded-tr-[8px] top-[226px] w-[800px]" data-name="Grill">
        <div className="absolute inset-0 pointer-events-none rounded-[inherit] shadow-[inset_0px_4px_4px_0px_rgba(0,0,0,0.25),inset_0px_4px_8.3px_4px_rgba(0,0,0,0.23)]" />
      </div>
      <p className="absolute font-['Manrope:Light',sans-serif] font-light leading-[normal] left-[767px] text-[#d6d6d6] text-[48px] top-[315px]">AMP</p>
      <Knob4 />
      <Knob5 />
      <Knob6 />
      <Knob7 />
      <Knob8 />
      <Knob9 />
      <Knob10 />
      <Knob11 />
      <Knob12 />
    </div>
  );
}

function Speaker() {
  return (
    <div className="absolute h-[179px] left-[1288px] top-[192px] w-[186px]" data-name="Speaker">
      <div className="absolute inset-[0_-2.15%_-4.47%_-2.15%]">
        <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 194 187">
          <g id="Speaker">
            <g filter="url(#filter0_d_1_117)" id="Ellipse 4">
              <ellipse cx="97" cy="89.5" fill="var(--fill-0, #202020)" rx="93" ry="89.5" />
            </g>
            <g filter="url(#filter1_d_1_117)" id="Ellipse 1">
              <ellipse cx="97" cy="89.5" fill="url(#paint0_radial_1_117)" rx="85" ry="81.5" />
              <path d={svgPaths.p385fad80} stroke="var(--stroke-0, #565656)" strokeOpacity="0.57" />
            </g>
            <path d={svgPaths.pd220600} id="Ellipse 2" stroke="var(--stroke-0, #090909)" strokeWidth="3" />
            <g filter="url(#filter2_d_1_117)" id="Ellipse 3">
              <ellipse cx="97" cy="89.5" fill="url(#paint1_radial_1_117)" fillOpacity="0.74" rx="36" ry="34.5" shapeRendering="crispEdges" />
            </g>
          </g>
          <defs>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="187" id="filter0_d_1_117" width="194" x="0" y="0">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="4" />
              <feGaussianBlur stdDeviation="2" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.25 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_117" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_117" mode="normal" result="shape" />
            </filter>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="176" id="filter1_d_1_117" width="183" x="5.5" y="2.5">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="1" />
              <feGaussianBlur stdDeviation="3.25" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.67 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_117" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_117" mode="normal" result="shape" />
            </filter>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="77" id="filter2_d_1_117" width="80" x="57" y="55">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="4" />
              <feGaussianBlur stdDeviation="2" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.25 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_117" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_117" mode="normal" result="shape" />
            </filter>
            <radialGradient cx="0" cy="0" gradientTransform="translate(97 89.5) rotate(90) scale(81.5 85)" gradientUnits="userSpaceOnUse" id="paint0_radial_1_117" r="1">
              <stop />
              <stop offset="1" stopColor="#262626" />
            </radialGradient>
            <radialGradient cx="0" cy="0" gradientTransform="translate(97 89.5) rotate(90) scale(34.5 36)" gradientUnits="userSpaceOnUse" id="paint1_radial_1_117" r="1">
              <stop stopColor="#444444" />
              <stop offset="1" stopColor="#151515" />
            </radialGradient>
          </defs>
        </svg>
      </div>
    </div>
  );
}

function Speaker1() {
  return (
    <div className="absolute h-[179px] left-[1288px] top-[451px] w-[186px]" data-name="Speaker 2">
      <div className="absolute inset-[0_-2.15%_-4.47%_-2.15%]">
        <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 194 187">
          <g id="Speaker">
            <g filter="url(#filter0_d_1_117)" id="Ellipse 4">
              <ellipse cx="97" cy="89.5" fill="var(--fill-0, #202020)" rx="93" ry="89.5" />
            </g>
            <g filter="url(#filter1_d_1_117)" id="Ellipse 1">
              <ellipse cx="97" cy="89.5" fill="url(#paint0_radial_1_117)" rx="85" ry="81.5" />
              <path d={svgPaths.p385fad80} stroke="var(--stroke-0, #565656)" strokeOpacity="0.57" />
            </g>
            <path d={svgPaths.pd220600} id="Ellipse 2" stroke="var(--stroke-0, #090909)" strokeWidth="3" />
            <g filter="url(#filter2_d_1_117)" id="Ellipse 3">
              <ellipse cx="97" cy="89.5" fill="url(#paint1_radial_1_117)" fillOpacity="0.74" rx="36" ry="34.5" shapeRendering="crispEdges" />
            </g>
          </g>
          <defs>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="187" id="filter0_d_1_117" width="194" x="0" y="0">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="4" />
              <feGaussianBlur stdDeviation="2" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.25 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_117" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_117" mode="normal" result="shape" />
            </filter>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="176" id="filter1_d_1_117" width="183" x="5.5" y="2.5">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="1" />
              <feGaussianBlur stdDeviation="3.25" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.67 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_117" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_117" mode="normal" result="shape" />
            </filter>
            <filter colorInterpolationFilters="sRGB" filterUnits="userSpaceOnUse" height="77" id="filter2_d_1_117" width="80" x="57" y="55">
              <feFlood floodOpacity="0" result="BackgroundImageFix" />
              <feColorMatrix in="SourceAlpha" result="hardAlpha" type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" />
              <feOffset dy="4" />
              <feGaussianBlur stdDeviation="2" />
              <feComposite in2="hardAlpha" operator="out" />
              <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.25 0" />
              <feBlend in2="BackgroundImageFix" mode="normal" result="effect1_dropShadow_1_117" />
              <feBlend in="SourceGraphic" in2="effect1_dropShadow_1_117" mode="normal" result="shape" />
            </filter>
            <radialGradient cx="0" cy="0" gradientTransform="translate(97 89.5) rotate(90) scale(81.5 85)" gradientUnits="userSpaceOnUse" id="paint0_radial_1_117" r="1">
              <stop />
              <stop offset="1" stopColor="#262626" />
            </radialGradient>
            <radialGradient cx="0" cy="0" gradientTransform="translate(97 89.5) rotate(90) scale(34.5 36)" gradientUnits="userSpaceOnUse" id="paint1_radial_1_117" r="1">
              <stop stopColor="#444444" />
              <stop offset="1" stopColor="#151515" />
            </radialGradient>
          </defs>
        </svg>
      </div>
    </div>
  );
}

export default function Frame() {
  return (
    <div className="relative size-full">
      <div className="absolute left-[469px] size-[32px] top-[300px]">
        <div className="absolute inset-[1.45%_6.7%]">
          <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 27.7128 31.0718">
            <path d={svgPaths.p2cfe6b00} fill="var(--fill-0, black)" id="Polygon 1" />
          </svg>
        </div>
      </div>
      <MainWindow />
      <Pedal />
      <Amp />
      <Speaker />
      <Speaker1 />
      <div className="absolute bg-[#343333] border border-[rgba(63,63,63,0.7)] border-solid h-[30px] left-[634px] rounded-[5px] shadow-[0px_4px_7.7px_-2px_rgba(0,0,0,0.12)] top-[135px] w-[365px]" data-name="amp load box" />
      <div className="absolute bg-[#343333] border border-[rgba(63,63,63,0.7)] border-solid h-[30px] left-[1281px] rounded-[5px] shadow-[0px_4px_7.7px_-2px_rgba(0,0,0,0.12)] top-[135px] w-[200px]" data-name="ir load box 1" />
      <div className="absolute bg-[#343333] border border-[rgba(63,63,63,0.7)] border-solid h-[30px] left-[1281px] rounded-[5px] shadow-[0px_4px_7.7px_-2px_rgba(0,0,0,0.12)] top-[672px] w-[200px]" data-name="ir load box 2" />
      <div className="absolute bg-[#343333] border border-[rgba(63,63,63,0.7)] border-solid h-[30px] left-[84px] rounded-[5px] shadow-[0px_4px_7.7px_-2px_rgba(0,0,0,0.12)] top-[135px] w-[240px]" data-name="drive load box" />
      <div className="absolute bg-[#202020] h-[6px] left-[1301px] rounded-[5px] top-[408px] w-[160px]">
        <div className="absolute inset-0 pointer-events-none rounded-[inherit] shadow-[inset_0px_4px_4px_0px_rgba(0,0,0,0.25)]" />
      </div>
      <div className="absolute bg-[#232323] blur-[0.25px] border border-[#1f1f1f] border-solid h-[34px] left-[1373px] rounded-[5px] top-[394px] w-[15px]" />
      <div className="absolute bg-[#d9d9d9] h-[28px] left-[1380px] top-[397px] w-px" />
    </div>
  );
}